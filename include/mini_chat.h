/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_chat.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:56:22 by njennes           #+#    #+#             */
/*   Updated: 2022/05/08 16:13:21 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_CHAT_H
# define MINI_CHAT_H

#include <netinet/in.h>

# define SERVER_MODE 0
# define JOIN_MODE 1

typedef struct connexion_handler_s
{
	int					socket_fd;
	struct sockaddr_in	client;
	pthread_t			worker;
}	connexion_handler_t;

void init_program();
void start_program();
void end_program(int code);

void start_server();
void close_server();

char *get_input_ip(void);
int get_chat_mode();

#endif