/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 08:56:52 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/27 09:07:26 by mbotelho         ###   ########.fr       */
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
	int		pn;
	long	number;

	i = 0;
	pn = 1;
	number = 0;
	if (!nptr)
		return (0);
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			pn = -pn;
	while (nptr[i] && is_digit(nptr[i]))
		number = number * 10 + (nptr[i++] - '0');
	return (number * pn);
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
