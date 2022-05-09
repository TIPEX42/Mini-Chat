/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:22:07 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 13:46:10 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "mini_chat.h"
#include "leaky.h"

char *name;

char *get_name();

int main()
{
	init_program();

	start_program();
	end_program(0);
	return (0);
}

void start_program()
{
	name = get_name();

	if (get_chat_mode() == SERVER_MODE)
		start_server();
	else
		start_client();
}

char *get_name()
{
	printf("Enter your name: ");
	fflush(stdout);

	char *user_input = gc_get_next_line(STDIN_FILENO);
	trim_input(user_input);
	while (user_input && strlen(user_input) == 1)
	{
		printf("Invalid name!\n");
		printf("Enter your name: ");
		fflush(stdout);

		user_input = gc_get_next_line(STDIN_FILENO);
		trim_input(user_input);
	}
	return (user_input);
}

void end_program(int code)
{
	close_server();
	gc_clean();
	exit(code);
}
