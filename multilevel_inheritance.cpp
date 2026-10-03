#include<iostream>

using namespace std;

class Emertxe //parent class
{
    protected:
        int id;
        string name;
        int dob;
    public:
        Emertxe(int id=0,string name="Nota",int dob =1000) :  id(id) ,name(name) , dob(dob) 
        {
            cout<<"parent class constructor called"<<endl;
        }
        void display()
        {
            cout<<"ID : "<<id<<endl
                <<"Name :"<<name<<endl
                <<"DOB : "<<dob<<endl;

        }
        ~Emertxe()
        {
            cout<<"Parent Destructor called"<<endl;
        }
};
class Student : public Emertxe   //student is child class 
{

 protected:
    int marks;
    
    public:
      Student(int id=200,string name="Navi",int dob=1992,int marks=29) : Emertxe(id,name,dob)//*Parameters passed from child to parent*/,marks(marks)
      {
        cout<<"Child constructor called"<<endl;
      }
      void displayMarks()
      {
        cout<<"Marks : "<<marks<<endl<<endl;
      }
      ~Student()
        {
            cout<<"Child 1 Destructor called"<<endl;
        }
};
class Mentor : public Student
{
 protected:
    int Noofbatches;

public:
    Mentor(int id=1,string name="Manu",int dob=1993,int Noofbatches=10) : Noofbatches(Noofbatches)
    {
        cout<<"Child class 2 is called"<<endl;

    }
    void displayAll()
    {
        display();
        displayMarks();
        cout<<"NO of batches :"<<Noofbatches<<endl<<endl;
    }
    ~Mentor()
    {
        cout<<"Child 2 Destructor called"<<endl;
    }
};
int main()
{
    
}