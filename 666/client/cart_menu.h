#ifndef CART_MENU_H
#define CART_MENU_H

void cart_menu_loop(int sockfd);
void add_to_cart();
void view_cart();
void remove_from_cart();
void checkout_cart();

#endif