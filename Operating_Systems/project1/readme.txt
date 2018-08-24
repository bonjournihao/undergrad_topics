void ResetCanonicalMode(int fd, struct termios *savedattributes){
    tcsetattr(fd, TCSANOW, savedattributes);
}//from Prof. Nitta's example


void SetNonCanonicalMode(int fd, struct termios *savedattributes);
//from Prof. Nitta's example