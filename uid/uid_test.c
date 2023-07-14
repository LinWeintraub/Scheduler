/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       0.0.1                                           *
 ~ Date          24/10/22                                        ~
 * Description   Unique identifier (UID)                         *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdio.h> /*printf*/

#include "uid.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"

#define TEST(name, actual, expected)\
    printf("%s: %s\n", name, actual == expected ? GREEN"Pass"WHITE : RED"Fail"WHITE)
    

int main(void)
{
    uid_ty my_uid1;
    uid_ty my_uid2;
    
    
    my_uid1 = UIDCreate();
    my_uid2 = UIDCreate();
    
    TEST("Is same test", UIDIsEqual(my_uid1, my_uid2), 0);
    
    printf("my_uid1.pid: %d\n" ,my_uid1.pid);
    printf("my_uid1.time: %li\n" ,my_uid1.time);
    printf("my_uid1.counter: %lu\n" ,my_uid1.counter);
    printf("my_uid1.ip_address: %s\n" ,my_uid1.ip_address);
    
    printf("\n");
    
    printf("my_uid2.pid: %d\n" ,my_uid2.pid);
    printf("my_uid2.time: %li\n" ,my_uid2.time);
    printf("my_uid2.counter: %lu\n" ,my_uid2.counter);
    printf("my_uid2.ip_address: %s\n" ,my_uid2.ip_address);


    return (0);
}
