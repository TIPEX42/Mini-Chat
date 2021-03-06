/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njennes <njennes@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/08 15:12:01 by njennes           #+#    #+#             */
/*   Updated: 2022/05/09 13:57:07 by njennes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "leaky.h"
#include "mini_chat.h"

int gc_callback(void *unused);

void init_program()
{
	gc_set_callback(gc_callback);
	init_signal();
}

int gc_callback(void *unused)
{
	(void)unused;
	gc_clean();
	exit(1);
}
