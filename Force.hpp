struct Force
{
	double x;
	double y;

    void operator= (Force& other)
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
