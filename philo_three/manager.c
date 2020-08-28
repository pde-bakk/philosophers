/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manager.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pde-bakk <pde-bakk@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/08/17 20:39:21 by pde-bakk      #+#    #+#                 */
/*   Updated: 2020/08/28 17:01:51 by pde-bakk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	genocide(t_data *data)
{
	int i;

	i = 0;
	sem_wait(data->finished);
	while (data->pids[i])
	{
		kill(data->pids[i], SIGTERM);
		++i;
	}
	i = 0;
	while (data->pids[i])
	{
		waitpid(data->pids[i], NULL, 0);
		++i;
	}
}

void	philosopher_death(t_philo *phil)
{
	sem_wait(phil->data->pen);
	ft_put_ul_fd(get_time_ms() - phil->data->starttime, 1);
	ft_putchar_fd('\t', 1);
	ft_put_ul_fd(phil->id, 1);
	ft_putchar_fd(' ', 1);
	ft_putstr_fd("has died", 1, 0);
	ft_putchar_fd('\n', 1);
}

void	*mr_manager(void *param)
{
	t_philo	*phil;

	phil = param;
	usleep(50);
	while (1)
	{
		sem_wait(phil->state_sem);
		if (get_time_ms() - phil->last_ate >=
			(unsigned long)phil->data->time_to_die)
		{
			philosopher_death(phil);
			sem_post(phil->data->finished);
			return (0);
		}
		sem_post(phil->state_sem);
		usleep(50);
	}
	return (0);
}
