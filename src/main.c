/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:22:07 by njennes           #+#    #+#             */
/*   Updated: 2022/05/08 15:51:43 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "mini_chat.h"
#include "leaky.h"

int get_chat_mode();

int main()
{
	init_program();

	start_program();

	char *chat_ip = get_input_ip();
	while (!chat_ip)
		chat_ip = get_input_ip();
	printf("Ip chosen: %s\n", chat_ip);

	end_program(0);
	return (0);
}

void start_program()
{
	if (get_chat_mode() == SERVER_MODE)
		start_server();
}

void end_program(int code)
{
	close_server();
	gc_clean();
	exit(code);
}
