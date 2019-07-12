#include <pthread.h>

class ReadWriteLock
{
private:
    pthread_mutex_t wr_mutex;
    pthread_cond_t wr_condition;

    int read_count;
    int write_count;

public:
    ReadWriteLock() : read_count(0), write_count(0)
    {
        pthread_mutex_init(&wr_mutex, NULL);
        pthread_cond_init(&wr_condition, NULL);
    }

    void readLock()
    {
        pthread_mutex_lock(&wr_mutex);

        //读操作数+1
        read_count++;

        //如果还有写操作，则阻塞，等待信号
        while (write_count > 0)
        {
            pthread_cond_wait(&wr_condition, &wr_mutex);
        }

        pthread_mutex_unlock(&wr_mutex);
    }

    void readUnlock()
    {
        pthread_mutex_lock(&wr_mutex);

        //读操作数-1
        read_count--;

        //如果读操作数为0，则释放信号量，让写操作能进行
        if (read_count == 0)
        {
            pthread_cond_signal(&wr_condition);
        }

        pthread_mutex_unlock(&wr_mutex);
    }

    void writeLock()
    {
        pthread_mutex_lock(&wr_mutex);

        //写操作数+1
        write_count++;

        //如果存在读操作，或写操作还没有完成，则等待全部完成后才获得锁
        while (write_count + read_count >= 2)
        {
            pthread_cond_wait(&wr_condition, &wr_mutex);
        }

        pthread_mutex_unlock(&wr_mutex);
    }

    void writeUnlock()
    {
        pthread_mutex_lock(&wr_mutex);

        //写操作数-1
        write_count--;

        //已经没有写操作，释放信号，通知读操作可以进行
        if (write_count == 0)
        {
            pthread_cond_signal(&wr_condition);
        }

        pthread_mutex_unlock(&wr_mutex);
    }
};