#include "commons.h"

t_list	*lstnew(void *data)
{
	t_list	*l;

	l = malloc(sizeof(t_list));
	if (!l)
		return (NULL);
	l->data = data;
	l->next = NULL;
	return (l);
}

static t_list	*lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	if (lst->next == NULL)
		return (lst);
	return (lstlast(lst->next));
}

void	lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = lstlast(*lst);
	last->next = new;
}

static void	lstdelone(t_list *lst)
{
	if (!lst)
		return ;
	if (lst->data)
		free(lst->data);
	free(lst);
}

void	lstclear(t_list **lst)
{
	t_list	*tmp;

	if (!lst)
		return ;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		lstdelone(*lst);
		*lst = tmp;
	}
	lst = NULL;
}

int	lstsize(t_list *lst)
{
	if (lst == NULL)
		return (0);
	return (lstsize(lst->next) + 1);
}