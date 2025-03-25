#pragma once

typedef struct s_list {
	void			*data;
	struct s_list	*next;
}	t_list;

t_list	*lstnew(void *data);
void	lstadd_back(t_list **lst, t_list *new);
void	lstclear(t_list **lst);
int		lstsize(t_list *lst);
