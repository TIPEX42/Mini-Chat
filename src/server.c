/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 15:22:39 by njennes           #+#    #+#             */
/*   Updated: 2022/05/08 17:29:52 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>
#include "mini_chat.h"
#include "leaky.h"

# define SERVER_BACKLOG 3

static int server_main_socket_fd = 0;
static pthread_t server_main_thread = {0};

static size_t workers_count = 0;
static connexion_handler_t *workers = NULL;

static void init_server_socket();
static void	*accept_connexions(void *unused);
static void handle_new_connexion(int new_socket, struct sockaddr_in client);
static void *worker_routine(void *ptr);

void start_server()
{
	init_server_socket();

	if (pthread_create(&server_main_thread, NULL, accept_connexions, NULL))
	{
		printf("Could not start the server main thread!\n");
		end_program(1);
	}
}

static void init_server_socket()
{
	server_main_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_main_socket_fd == -1)
	{
		printf("Error: Could not create a socket!\n");
		end_program(1);
	}

	struct sockaddr_in server = {0};
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8989);

	if(bind(server_main_socket_fd,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("Error: Could not bind the server socket!\n");
		end_program(1);
	}

	listen(server_main_socket_fd, SERVER_BACKLOG);
}

static void	*accept_connexions(void *unused)
{
	(void)unused;

	struct sockaddr_in client = {0};
	socklen_t c = sizeof(struct sockaddr_in);

	int new_socket = accept(server_main_socket_fd, (struct sockaddr *)&client, &c);
	if (new_socket < 0)
		printf("An incoming connexion failed!\n");

	char *client_ip = inet_ntoa(client.sin_addr);
	int client_port = ntohs(client.sin_port);
	printf("Incoming connexion from: %s : %d\n", client_ip, client_port);
	handle_new_connexion(new_socket, client);

	return (NULL);
}

static void handle_new_connexion(int new_socket, struct sockaddr_in client)
{
	connexion_handler_t new_worker = {0};
	new_worker.socket_fd = new_socket;
	new_worker.client = client;

	if (!workers)
		workers = gc_calloc(1, sizeof (connexion_handler_t));
	else
	{
		connexion_handler_t *new_workers = gc_calloc(workers_count + 1, sizeof (connexion_handler_t));
		gc_memmove(new_workers, workers, workers_count * sizeof (connexion_handler_t));
		gc_free(workers);
		workers = new_workers;
	}

	workers[workers_count] = new_worker;
	if (pthread_create(&workers[workers_count].worker, NULL, worker_routine, &workers[workers_count]))
	{
		printf("Could not start a new thread for the incomming connexion!\n");
		end_program(1);
	}
	pthread_detach(workers[workers_count].worker);
	workers_count++;
}

static void *worker_routine(void *ptr)
{
	connexion_handler_t *worker = (connexion_handler_t *)ptr;

	char *message = "Hello Client , I have received your connection. But I have to go now, bye\n";
	write(worker->socket_fd , message , strlen(message));
	close(worker->socket_fd);
	return (NULL);
}

void close_server()
{
	if (server_main_socket_fd)
		close(server_main_socket_fd);

	if (workers)
	{
		size_t i = 0;
		while (i < workers_count)
		{
			close(workers[i].socket_fd);
			if (workers[i].worker)
				pthread_join(workers[i].worker, NULL);
			i++;
		}
		gc_free(workers);
	}
}
