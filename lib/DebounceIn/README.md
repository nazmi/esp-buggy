# DebounceIn
Debounce InterruptIn for mbed. Attached callbacks are ISR, meaning only non-blocking code is allowed there.

## Example

```cpp
#include "mbed.h"
#include "DebounceIn.h"

DebounceIn button(PE_3);

void buttonRise(){
    // note that printf is not allowed in interrupt, this is an example
    printf("btn rise\n");
}

void buttonFall(){
    // note that printf is not allowed in interrupt, this is an example
    printf("btn Fall\n");
}

int main() {
    button.rise(callback(buttonRise));
    button.fall(callback(buttonFall));

    bool current_state = button;

    printf("state: %u\n", sd_inserted);

    while (1) {}
}
```
