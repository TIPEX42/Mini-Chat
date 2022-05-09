/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:17:01 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 13:20:00 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mini_chat.h"

char *read_packet(packet_t *packet, int socket_fd)
{
	ssize_t bytes_read = read(socket_fd, packet, sizeof (packet_t));
	if (bytes_read <= 0)
		return (NULL);

	char *packet_content = calloc(packet->packet_size + 1, sizeof (char));
	if (!packet_content)
		end_program(1);

	ssize_t total_bytes_read = 0;
	while (total_bytes_read < packet->packet_size)
	{
		bytes_read = read(socket_fd, packet_content + total_bytes_read, packet->packet_size - total_bytes_read);
		if (bytes_read <= 0)
		{
			free(packet_content);
			return (NULL);
		}
		total_bytes_read += bytes_read;
	}

	return (packet_content);
}

void send_packet(int socket_fd, int packet_type, char *content, int bytes)
{
	packet_t packet = {0};
	packet.packet_type = packet_type;
	packet.packet_size = bytes;

	write(socket_fd, &packet, sizeof (packet));
	write(socket_fd, content, bytes);
}
