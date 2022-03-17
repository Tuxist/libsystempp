const char* std::exception::what(){
    std::size_t bufsize=0,written=0;
    for(Message *curmsg=_firstMessage; curmsg; curmsg=curmsg->_nextMessage){
        bufsize+=curmsg->_BufferSize;
    }
    delete[] _printBuffer;
    _printBuffer = new char[bufsize+1];
    for(Message *curmsg=_firstMessage; curmsg; curmsg=curmsg->_nextMessage){
        scopy(curmsg->_Buffer,curmsg->_Buffer+curmsg->_BufferSize,_printBuffer+written);
        written+=curmsg->_BufferSize;
    }
    _printBuffer[bufsize]='\0';
    
    return _printBuffer;
}
