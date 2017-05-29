





### Using a C extension in Python for Morse Code

Use getch() to time keydown.


Issues:

1. Debounce - can be managed in C for very short intervals
    - `very short`: faster than human tap speeds, represent hardware issues
    - `getch` just gets a character, so every getch must be timed and debounced
    - if the getch does not qualify as a debounce, don't return anything yet and don't reset the timer.


Outputs:

- When getch() begins returning the morse code token, return the amount of time the key was down 
- The processing application can get the current time and use that to determine all other information.


