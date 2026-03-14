## Strings Overview
Ok with all these strings it might get a little confusing, so I'll break each one down below. 

Each of my custom string classes have some really nice properties that makes it super easy to pass around data. For example, lets say I have an `hstring` and I want to store all the data from  my `Sprite s`. What I can do is this:
```c++
hstring my_string(50); // Allocates 50 characters to the heap
my_string << sprite.pos.x << ' ' << sprite.pos.y << '\n';
my_string << sprite.animnum << ",\t" << sprite.framenum; 
```
Thus all the data is immediately converted into the hstring formatted like `"30.0 40.0\n3,\t4"`. Furthermore, I can add even nicer features like:
```c++
mystring << trunc<4>() << my_float << gan::endl;
```
Now, my_float will be truncated to four digits automatically without me having to do any annoying calculations. Now to get on to each type of string.
### fstrings  
First up we have `fstring`, which stands for **fixed string**. `fstring` is a templatede class, and this means that upon creation, these strings have completely **fixed length** that cannot be changed. These are great for text buffers that get refreshed repeatedly, and for ensuring compliance with specific string lengths. However, the main benefits are below:
- Significantly faster for writing and conversion than regular `std::string` (around 2x - 3x faster)
- Lives on the stack, so heap fragmentation (especially in WASM) is not a concern for fstrings
- Prevents any overflow automatically.

For example, the `gan::Text` class is templated with an `fstring`, because in reality, displayed text on screen should never be unbounded, and when users make a text object, they should define how it will be used.

Note that `fstrings` have an internal limit of 65442 characters as a built in cap. Creating more than that many characters is bad for the stack and should not happen. 
- Downsides: You must know your string length at compile time, so don't use this for managing something like a pathname. (However, it is good for preventing users from adding too much input.)
- You can't pass fstrings through virtual functions cause' they're templated. However, there's a real good solution for this in the `str_view` section.

Here's an example of how to make an fstring:
```c++
gan::fstring<10> fstr; // creates fstring with 10 char limit;
fstr << "Hello World!\n"; // pipes in "hello world!\n" (14 chars)
std::cerr << fstr.c_str() << std::endl;
```
Our output from this will be `Hello worl`, because we only allocated 10 characters. 

**Warning:** `fstring` always allocates two additional bytes past what you specify to the internal character array. One is for the null terminator (at size + 0), and the next is for floating point conversion information (at size + 1).

### hstrings
Ok we'll keep this short. `hstrings` are just `fstrings` but their memory is allocated on the heap, and they don't have a fixed size. 

#### Pros:
- Still faster than regular `std::string`, but not by as much as `fstring`
- Dynamically resizable & virtually unlimited size
- Access to all the nice operator overloads `fstring` has. 
#### Cons:
- It's on the heap, so fragmentation can be a problem
- Slower than `fstring`
- Higher memory baseline than `fstring`
- Constantly allocates new memory, which is really bad for WASM environment
- Cannot limit it's own size. It's buffer will grow to the C++ 32-bit unsigned int limit. 
- Not memory-safe

Thus, for most general cases, please please please use an `fstring` as they're so much better for engine performance than hstrings. Though there are several cases  where `hstrings` are the best answer:
1. Path names
2. System-wide data collection
3. That's pretty much it.

Moral of the story: use fstring. Mostly anything in a game engine is predictable enough that you can predict close to the number of characters you'll need before compilation.
### str_view
Now this is the crown-jewel of the string implementation. You might think "it's hella uncool to try and manage two different string types when writing code", and you'd be right. Hence, we have the `str_view` (name generously borrowed from the standard library). 

Instead of passing `fstring` and `hstring` through functions, you pass `str_view` instead, and you do this by "wrapping" the strings.
```c++
void add_to_buffer(str_view buffer, float my_float) {
    // adds a float to the string with a precision of 4
    buffer << trunc<4>() << my_float; 
}

int main() {
    // create our strings
    fstring<300> fixed_string = "My fixed string: ";
    hstring heap_string = "My dynamically allocated string: ";
    // now we add to buffer
    add_to_buffer(fixed_string.wrap(), 30.1234567f);
    add_to_buffer(heap_string.wrap(), 90.1234567f);
    // print to console
    std::cerr << fixed_string.c_str() << std::endl;
    std::cerr << heap_string.c_str() << std::endl;
}
```
```commandline
output:
My fixed string: 30.1234
My heap string: 90.1234
```
Now you may be wondering, "how does this work"? And i'll say its magic cause i dont wanna bore you with the implementation. (it's just references & pointers, and a shit ton of operator overloads).
### Usage
`hstring` and `fstring` have the same member functions, so i'll cover them together.
- `capacity()`› Returns current capacity of the string
- `length()` › Returns length of the string
- `cstr()`› Returns a C-style string (usefull for printing to std::cout)
- `clear()` › Clears out the string. Does not resize
- `operator[](index i)` › Accesses at index `i`

That's pretty much it. You also have a ton of operator overloads as well:
- `==` › Works from (`str_view` compatable):
  - `fstring ‹-› fstring`
  - `hstring ‹-› hstring`
  - `fstring ‹-› const char[]`
  - `hstring ‹-› const char[]`
- `<<` › Works with (`str_view` compatable):
  - `f/hstring ‹-› bool, int, float, double, void*, f/hstring`
- `+` › Works with
  - `fstring ‹-› fstring` › Creates new fstring of combined capacity, so be careful.

`str_view` also has one specialty function: `subview(uint32_t start, uint32_t sub_len)`. This creates a const `str_subview` object that has access to a singular operator overload:
`== const char[]`.

You might ask, what the fuck? Why would you make that? Simple reason: syntax highlighting. Let's say you have a text parser who takes in a str_view object. `.subview` acts as an equivalent to `.substr`, except there's no dynamic memory allocation, and extremely quick comparison to string literals. 

Subview (and the rest of this string suite) is optimized to use any information, like the length of a constant literal, to it's advantage. Thus, comparisons between `str_subview` objects and const char objects are rapid. Most of the time you don't even need to store the created `str_subview` object. For example:
```c++
for (int i = 0; i < my_str_view.length(); i++) {
    if (my_str_view.subview(i, 4) == "true") {
        color_text("blue");
        i+=3;
        continue;
    }
}
```
Isn't that nifty. That's all i have to say for now bye.