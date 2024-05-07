struct Position
{
	double x;
	double y;

    void operator= (Position& other)
    {
        x = other.x;
        y = other.y;
    }

    void setZeros()
    {
        x = 0;
        y = 0; 
    }
};
