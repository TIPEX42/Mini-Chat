/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 15:22:39 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 14:34:44 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include "mini_chat.h"

# define SERVER_BACKLOG 3
# define MAX_CONNECTED_CLIENTS 100

static int server_main_socket_fd = 0;
static pthread_t server_main_thread = {0};

static size_t clients_count = 0;
static connected_client_t clients[MAX_CONNECTED_CLIENTS];
static pthread_mutex_t workers_mutex = {0};

static void init_server_socket();
static void	*accept_connexions(void *unused);
static void handle_new_connexion(int new_socket, struct sockaddr_in client);
static void *worker_routine(void *ptr);
static void remove_client(connected_client_t *client);
static void broadcast(int type, char *client_name, char *content);

void start_server()
{
	init_server_socket();
	pthread_mutex_init(&workers_mutex, NULL);

	if (pthread_create(&server_main_thread, NULL, accept_connexions, NULL))
	{
		printf("Could not start the server main thread!\n");
		end_program(1);
	}
	pthread_detach(server_main_thread);
	while (1);
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
	server.sin_port = htons(8991);

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

	while (1)
	{
		struct sockaddr_in client = {0};
		socklen_t len = sizeof(struct sockaddr_in);

		int new_socket = accept(server_main_socket_fd, (struct sockaddr *)&client, &len);
		if (new_socket < 0)
		{
			printf("An incoming connexion failed! : %s %d\n", strerror(errno), server_main_socket_fd);
			return (NULL);
		}

		char *client_ip = inet_ntoa(client.sin_addr);
		int client_port = ntohs(client.sin_port);
		printf("Incoming connexion from: %s : %d\n", client_ip, client_port);
		handle_new_connexion(new_socket, client);
	}
}

static void handle_new_connexion(int new_socket, struct sockaddr_in client)
{
	connected_client_t new_worker = {0};
	new_worker.socket_fd = new_socket;
	new_worker.socket_infos = client;
	new_worker.ip = inet_ntoa(client.sin_addr);
	new_worker.port = ntohs(client.sin_port);

	pthread_mutex_lock(&workers_mutex);
	if (clients_count == MAX_CONNECTED_CLIENTS)
	{
		printf("Connexion refused : The chat is full!\n");
		return ;
	}

	for (size_t i = 0; i < MAX_CONNECTED_CLIENTS; i++)
	{
		if (clients[i].socket_fd == 0)
		{
			clients[i] = new_worker;
			if (pthread_create(&clients[i].worker, NULL, worker_routine, &clients[i]))
			{
				printf("Could not start a new thread for the incomming connexion!\n");
				end_program(1);
			}
			break ;
		}
	}

	pthread_detach(clients[clients_count].worker);
	clients_count++;
	pthread_mutex_unlock(&workers_mutex);
}

static void remove_client(connected_client_t *client)
{
	pthread_mutex_lock(&workers_mutex);
	close(client->socket_fd);
	free(client->name);
	memset(client, 0, sizeof(connected_client_t));
	clients_count--;
	pthread_mutex_unlock(&workers_mutex);
}

static void *worker_routine(void *ptr)
{
	connected_client_t *client = (connected_client_t *)ptr;

	packet_t packet = {0};
	char *packet_content = read_packet(&packet, client->socket_fd);
	while (packet_content)
	{
		if (packet.packet_type == PACKET_CONNECTION)
		{
			printf("Welcome %s!\n", packet_content);
			client->name = packet_content;
			broadcast(PACKET_CONNECTION, client->name, NULL);
		}
		else if (packet.packet_type == PACKET_MESSAGE_TO_SERVER)
		{
			printf("[%s]: %s\n", client->name, packet_content);
			broadcast(PACKET_MESSAGE_FROM_SERVER, client->name, packet_content);
			free(packet_content);
		}
		packet_content = read_packet(&packet, client->socket_fd);
	}

	printf("Client %s [%s:%d] disconnected\n", client->name, client->ip, client->port);
	remove_client(client);
	return (NULL);
}

static void broadcast(int type, char *client_name, char *content)
{
	if (type == PACKET_MESSAGE_FROM_SERVER)
	{
		int bytes = strlen(client_name) + 1 + strlen(content);
		char *full_content = calloc(bytes + 1, sizeof (char));
		if (!full_content)
			return ;
		memmove(full_content, client_name, strlen(client_name));
		memmove(full_content + strlen(client_name) + 1, content, strlen(content));

		size_t i = 0;
		while (i < MAX_CONNECTED_CLIENTS)
		{
			if (clients[i].socket_fd)
				send_packet(clients[i].socket_fd, PACKET_MESSAGE_FROM_SERVER, full_content, bytes);
			i++;
		}
		free(full_content);
	}
	else if (type == PACKET_CONNECTION)
	{
		size_t i = 0;
		while (i < MAX_CONNECTED_CLIENTS)
		{
			if (clients[i].socket_fd)
				send_packet(clients[i].socket_fd, PACKET_CONNECTION, client_name, strlen(client_name));
			i++;
		}
	}

}

void close_server()
{
	close(server_main_socket_fd);
	server_main_socket_fd = 0;

	pthread_mutex_lock(&workers_mutex);
	size_t i = 0;
	while (i < clients_count)
	{
		close(clients[i].socket_fd);
		if (clients[i].worker)
			pthread_join(clients[i].worker, NULL);
		i++;
	}
	pthread_mutex_unlock(&workers_mutex);
	pthread_mutex_destroy(&workers_mutex);
}
