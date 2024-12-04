# Benchmarking

## How to Test?

To test just run the following command -

```
.\test.ps1
```

## Working

I used a custom method to benchmark each functionality. The method followed was to use a **Timer** class whose constructor sets the start time and its destructor sets the end time.

```
class Timer{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;

public:
    Timer(){
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;

        std::cout << duration << "us (" << ms << "ms)\n\n";
    }
};
```

To use this class write the function to be tested in a block.

Example usage:

```
class Timer{
    // Class definition
};

int main(){
    {
        Timer timer;
        // This code gets timed
    }

    return 0;
}
```

The time for each such block is printed on the terminal.
