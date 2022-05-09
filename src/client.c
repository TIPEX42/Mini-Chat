/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:02:21 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 14:30:50 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include "mini_chat.h"

static int client_socket_fd;
static pthread_t client_thread;

static int connect_to(char *ip);
static void *client_read_messages(void *unused);

void start_client()
{
	char *chat_ip = get_input_ip();
	while (!chat_ip)
		chat_ip = get_input_ip();
	printf("Connection to %s...\n", chat_ip);

	if (!connect_to(chat_ip))
		return ;
}

static int connect_to(char *ip)
{
	client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket_fd < 0)
	{
		printf("Could not create the client socket!\n");
		end_program(1);
		return (0);
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_port = htons(8991);
	serv_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &serv_addr.sin_addr);

	if (connect(client_socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Connection to chat failed\n");
		return (0);
	}

	send_packet(client_socket_fd, PACKET_CONNECTION, name, strlen(name));

	if (pthread_create(&client_thread, NULL, client_read_messages, NULL))
	{
		printf("Could not create the client main thread!\n");
		end_program(1);
	}
	while (1);
	return (1);
}

static void *client_read_messages(void *unused)
{
	(void)unused;

	packet_t packet = {0};
	char *packet_content = read_packet(&packet, client_socket_fd);
	while (packet_content)
	{
		char *content = packet_content + strlen(packet_content) + 1;
		if (packet.packet_type == PACKET_CONNECTION)
			printf("Welcome %s!\n", packet_content);
		else if (packet.packet_type == PACKET_MESSAGE_FROM_SERVER)
			printf("[%s]: %s\n", packet_content, content);
		free(packet_content);
		packet_content = read_packet(&packet, client_socket_fd);
	}
	return (NULL);
}
