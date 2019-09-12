#ifndef TMYPOINT_H
#define TMYPOINT_H

namespace fteo
{


class TMyPoint
{
    public:
        TMyPoint();
        virtual ~TMyPoint();
        void func1();
      char   *Name;
        void SetName(char* _Name);
      void NewMethod(int TestNumber);
    protected:
    private:
};


#endif // TMYPOINT_H
}
