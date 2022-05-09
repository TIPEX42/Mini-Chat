/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 14:44:43 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 14:49:45 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "leaky.h"
#include "mini_chat.h"

static int is_ip_valid(char *ip);

char *get_input_ip(void)
{
	printf("Enter chat IP: ");
	fflush(stdout);
	char *input_ip = gc_get_next_line(STDIN_FILENO);
	trim_input(input_ip);

	if (is_ip_valid(input_ip))
		return (input_ip);
	printf("The IP given isn't valid! Format is: 0-255.0-255.0-255.0-255\n");
	gc_free(input_ip);
	return (NULL);
}


int get_chat_mode()
{
	printf("Do you want to host a chat or join one ?(host 0, join 1): ");
	fflush(stdout);

	char *user_input = gc_get_next_line(STDIN_FILENO);
	trim_input(user_input);

	while (!user_input || strlen(user_input) != 1
		   || !(*user_input == '0' || *user_input == '1'))
	{
		gc_free(user_input);
		printf("Invalid input!\n");
		printf("Do you want to host a chat or join one ?(host 0, join 1): ");
		fflush(stdout);
		user_input = gc_get_next_line(STDIN_FILENO);
		trim_input(user_input);
	}
	int final_choice = *user_input - '0';
	gc_free(user_input);
	return (final_choice);
}

static int is_ip_valid(char *ip)
{
	if (!ip)
		return (0);

	size_t i = 0;
	size_t blocks = 0;
	while (ip[i])
	{
		size_t digits = 0;
		while (ip[i] && isdigit(ip[i]))
		{
			i++;
			digits++;
			if (digits > 3)
				return (0);
		}
		if (digits == 0)
			return (0);
		blocks++;
		if (blocks < 3 && ip[i] != '.')
			return (0);
		i++;
	}
	if (blocks != 4)
		return (0);
	return (1);
}

void trim_input(char *input)
{
	size_t i = strlen(input) - 1;
	while (i >= 0 && isspace(input[i]))
	{
		input[i] = 0;
		i--;
	}
}
