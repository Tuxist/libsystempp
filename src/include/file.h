namespace libsystempp{
    class FileDescriptor {
    public:
        virtual int getState()=0;
        virtual int toInt()=0;
    protected:
        operator=(int value);
        int State;
    private:
        int _FileDescriptor;
    }
};
