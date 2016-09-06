
# ifndef                    BTHREAD_HPP_
# define                    BTHREAD_HPP_

# include                   <thread>

class                       BThread
{
private:
    std::thread             *_thread;

public:
    BThread()
        : _thread(nullptr)
    {

    }
    ~BThread()
    {

    }
private:
    BThread(BThread const &);
    BThread&                operator=(BThread const &);

public:
    template <typename T>
    void                    run(T object)
    {
        if (this->_thread != nullptr)
            this->_thread = new std::thread(std::ref(object));
    }
    bool                    isRun() const
    {
        return (!(this->_thread == nullptr));
    }
    void                    stop()
    {
        if (this->_thread != nullptr)
        {
            this->_thread->join();
            delete this->_thread;
            this->_thread = nullptr;
        }
    }
};

# endif                     /* !BTHREAD_HPP_ */