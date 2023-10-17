# ft_irc (server + client)
This project is one of the 42 common core. The goal of this project is to recreate a simple IRC server from scratch in a pure c++ 98 with basic commands. I also have build a client to test the server.<br><br>
Because **we can't use multithreading** to build the server I use a fifo [ first in first out ] system to treat the clients request. In addition to that, we have to **use non-blocking file descriptor**, that's why i choose to use `select()` witch is a tool to manage file descriptor (for more information, see [this](https://man7.org/linux/man-pages/man2/select.2.html)).<br>
To compile this project, jou just have to use `make`.
### Dependencies
None...
### How it works
To create the base of the server I take [this article](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/) as reference. Below you can find the skeleton of my server:<br>
```C++
while (true) {

  /* initiate all fd_set (1) */
  FD_ZERO(&readfds); // clear socket_set
  FD_ZERO(&writefds);
  FD_SET(fd, &readfds); // add server ID to socket_set

  /* add all client ID to socket_set and find the bigger fd (2) */
  max_sd = this->_fd;
  for (int i = 0; fd_list[i]; ++i) {
    if (fd_list[i] > 0) {
      FD_SET(fd_list[i], &readfds);
      /* only if this fd has to write (3) */
      FD_SET(sd, &writefds);
    }
    if (sd > max_sd)
      max_sd = sd;
  }

  struct timeval	tv = { 0, 50000 };
  activity = select(max_sd + 1, &readfds, &writefds, NULL, &tv);

  /* incoming connection (4) */
  if (FD_ISSET(fd, &readfds))
    addClient();

  /* I/O (input/output) operation from client (5) */
  for (int i = 0; fd_list[i]; ++i) {
    if (FD_ISSET(fd_list[i], &readfds))
      readFromClient(fd_list[i]);
  }

  for (int i = 0; fd_list[i]; ++i) {
    if (FD_ISSET(fd_list[i], &writefds))
      sendToClient(fd_list[i]);
  }
}
```
(1) For each time we iterate the loop, we have to initiate our fd_set(s) with FD_ZERO() to set fd_set to zero. We also have two fd_set, one to check if we can read on the fd and an other to check if we can write.<br>
(2) Then we initiate with FD_SET to add a file descriptor.<br>
(3) Important thing, [to avoid an explosion of our CPU usage](https://stackoverflow.com/questions/19738300/what-is-the-issue-of-select-using-so-much-cpu-power), simply define in `writefds` the fd witch must write.<br>
(4) After we have to check the result of select: if the server fd is ready to read, it means a client want to connect to the server.<br>
(5) We just have to check if we can read or write in our fd_list.<br>

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

Date: Oct 2023
