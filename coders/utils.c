/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 08:56:52 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/06 12:24:10 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atol(const char *nptr)
{
	int		i;
	long	number;
	long	max;

	i = 0;
	number = 0;
	max = 9223372036854775807;
	if (!nptr)
		return (0);
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	while (nptr[i] && is_digit(nptr[i]))
	{
		if (number > max / 10 || (number == max / 10 && (nptr[i] - '0') > max
				% 10))
			return (-1);
		number = number * 10 + (nptr[i++] - '0');
	}
	return (number);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		pn;
	long	number;

	i = 0;
	number = 0;
	if (!nptr)
		return (0);
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	while (nptr[i] && is_digit(nptr[i]))
	{
		number = number * 10 + (nptr[i++] - '0');
		if (number > 2147483647)
			return (-1);
	}
	return ((int)number);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = strlen(s);
	dest = malloc((s_len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = -1;
	while (++i < s_len)
		dest[i] = s[i];
	dest[i] = '\0';
	return (dest);
}

void	*ft_free(t_args *coders)
{
	if (!coders)
		return (NULL);
	if (coders->scheduler)
		free(coders->scheduler);
	free(coders);
	return (NULL);
}
