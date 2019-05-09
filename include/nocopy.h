namespace myftp
{
    class nocopyable
    {
        public:
            nocopyable(const nocopyable&)=delete;
            nocopyable&  operator=(const nocopyable)=delete;
        protected:
            nocopyable()=default;
           virtual ~nocopyable()=default;
    };
}