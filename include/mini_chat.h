/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_chat.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:56:22 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 13:56:58 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_CHAT_H
# define MINI_CHAT_H

#include <netinet/in.h>

# define SERVER_MODE 0
# define JOIN_MODE 1

# define PACKET_CONNECTION 0
# define PACKET_MESSAGE_TO_SERVER 1
# define PACKET_MESSAGE_FROM_SERVER 2

extern char *name;

typedef struct connected_client_s
{
	int					socket_fd;
	struct sockaddr_in	socket_infos;
	char				*ip;
	int					port;
	char				*name;
	pthread_t			worker;
}	connected_client_t;

typedef struct packet_s
{
	int	packet_type;
	int	packet_size;
}	packet_t;

void init_program();
void init_signal();
void start_program();
void end_program(int code);

void start_server();
void close_server();

void start_client();

char *get_input_ip(void);
int get_chat_mode();

void trim_input(char *input);

char *read_packet(packet_t *packet, int socket_fd);
void send_packet(int socket_fd, int packet_type, char *content, int bytes);

#endif