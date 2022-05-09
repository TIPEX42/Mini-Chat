/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 13:54:00 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 13:56:26 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "mini_chat.h"

void sig_handler(int signal);

void init_signal()
{
	signal(SIGINT, sig_handler);
}

void sig_handler(int signal)
{
	if (signal == SIGINT)
		end_program(0);
}
