/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgarcia- <rgarcia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 13:51:32 by rgarcia-          #+#    #+#             */
/*   Updated: 2021/02/16 14:01:38 by rgarcia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_zona
{
	int			width;
	int			height;
	char		fondo;
}				t_zona;

typedef struct	s_forma
{
	char		tipo;
	float		x;
	float		y;
	float		width;
	float		height;
	char		color;
}				t_forma;

int		ft_strlen(char *str)
{
	int i = 0;

	while (str[i] != '\0')
		i++;
	return (i);
}

int		ft_error(FILE *file, char *dibujo, char *error)
{
	if (file)
		fclose(file);
	if (dibujo)
		free(dibujo);
	if (error)
		write(1, error, ft_strlen(error));
	return (1);
}

void	ft_draw(char *dibujo, t_zona *zona)
{
	int	i = 0;

	while (i < zona->height)
	{
		write(1, dibujo + (i * zona->width), zona->width);
		write(1, "\n", 1);
		i++;
	}
}

char	*get_zona(FILE *file, t_zona *zona)
{
	int		i = 0;
	char	*dibujo;

	if ((fscanf(file, "%d %d %c\n", &zona->width, &zona->height, &zona->fondo)) != 3)
		return (0);
	if (zona->width <= 0 || zona->width > 300 || zona->height <= 0 ||
	zona->height > 300)
		return (0);
	if (!(dibujo = (char *)malloc(sizeof(*dibujo) * (zona->width * zona->height))))
		return (0);
	while (i < (zona->width * zona->height))
		dibujo[i++] = zona->fondo;
	return (dibujo);
}

int		in_rectangle(float x, float y, t_forma *forma)
{
	if (x < forma->x || forma->x + forma->width < x ||
	y < forma->y || forma->y + forma->height < y)
		return (0);
	if (x - forma->x < 1 || forma->x + forma->width - x < 1 ||
	y - forma->y < 1 || forma->y + forma->height - y < 1)
		return (2);
	return (1);
}

int		get_forma(FILE *file, char **dibujo, t_zona *zona)
{
	int		x = 0, y = 0, ret, ret_scan;
	t_forma	*forma;

	while ((ret_scan = fscanf(file, "%c %f %f %f %f %c\n", &forma->tipo, &forma->x,
	&forma->y, &forma->width, &forma->height, &forma->color)) == 6)
	{
		if (forma->width <= 0 || forma->height <= 0 ||
		(forma->tipo != 'r' && forma->tipo != 'R'))
			return (0);
		x = 0;
		while (x < zona->height)
		{
			y = 0;
			while (y < zona->width)
			{
				ret = in_rectangle(y, x, forma);
				if ((forma->tipo == 'r' && ret == 2) || (forma->tipo == 'R' && ret))
					(*dibujo)[x * zona->width + y] = forma->color;
				y++;
			}
			x++;
		}
	}
	if (ret_scan != -1)
		return (0);
	return (1);
}

int		main(int argc, char **argv)
{
	FILE	*file;
	t_zona	*zona;
	char	*dibujo;

	if (argc != 2)
		return (ft_error(NULL, NULL, "Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (ft_error(NULL, NULL, "Error: Operation file corrupted\n"));
	if (!(dibujo = get_zona(file, zona)))
		return (ft_error(file, NULL, "Error: Operation file corrupted\n"));
	if (!(get_forma(file, &dibujo, zona)))
		return (ft_error(file, NULL, "Error: Operation file corrupted\n"));
	ft_draw(dibujo, zona);
	ft_error(file, dibujo, NULL);
	return (0);
}
