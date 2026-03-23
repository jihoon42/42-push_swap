/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

t_node	*new_node(int data)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	push_front(t_deque *q, t_node *x)
{
	if (!x)
		return ;
	x->prev = NULL;
	x->next = q->front;
	if (q->front)
		q->front->prev = x;
	else
		q->back = x;
	q->front = x;
	q->size++;
}

void	push_back(t_deque *q, t_node *x)
{
	if (!x)
		return ;
	x->next = NULL;
	x->prev = q->back;
	if (q->back)
		q->back->next = x;
	else
		q->front = x;
	q->back = x;
	q->size++;
}

t_node	*pop_front(t_deque *q)
{
	t_node	*node;

	if (!q->front)
		return (NULL);
	node = q->front;
	q->front = node->next;
	if (q->front)
		q->front->prev = NULL;
	else
		q->back = NULL;
	node->next = NULL;
	node->prev = NULL;
	q->size--;
	return (node);
}

t_node	*pop_back(t_deque *q)
{
	t_node	*node;

	if (!q->back)
		return (NULL);
	node = q->back;
	q->back = node->prev;
	if (q->back)
		q->back->next = NULL;
	else
		q->front = NULL;
	node->next = NULL;
	node->prev = NULL;
	q->size--;
	return (node);
}
