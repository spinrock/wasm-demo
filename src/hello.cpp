#include <stdio.h>
#include <string>
#include <emscripten.h>

int main() {
    printf("Hello, world\n");
    return 0;
}

extern "C" void EMSCRIPTEN_KEEPALIVE initialized()
{
    puts("initialized");
}

extern "C" void EMSCRIPTEN_KEEPALIVE clicked1()
{
    puts("clicked1");
}

EM_JS(void *, getElementValue_, (char const *id),
{
    var e = document.getElementById(UTF8ToString(id));
    var str = e.value;
    console.log(str);
    var len = lengthBytesUTF8(str) + 1;
    var heap = _malloc(len);
    stringToUTF8(str, heap, len);
    return heap;
});

std::string getElementValue(std::string const &id)
{
    void *p = getElementValue_(id.c_str());
    std::string s((char const *)p);
    free(p);
    return s;
}

void setElementValue(std::string const &id, std::string const &value)
{
    EM_ASM({
        var e = document.getElementById(UTF8ToString($0));
        e.value = UTF8ToString($1);
    }, id.c_str(), value.c_str());
}

extern "C" void EMSCRIPTEN_KEEPALIVE clicked2()
{
    setElementValue("input1", "Hello, world");
}

void setElementInnerHTML(std::string const &id, std::string const &html)
{
    EM_ASM({
        var e = document.getElementById(UTF8ToString($0));
        e.innerHTML = e.innerHTML + UTF8ToString($1);
    }, id.c_str(), html.c_str());
}

extern "C" void EMSCRIPTEN_KEEPALIVE clicked3()
{
    setElementInnerHTML("contents", "<p>Hello, React world</p>");
}

void setElementInnerHTMLToHTML(std::string const &id, std::string const &id2)
{
    EM_ASM({
        var addTarget = document.getElementById(UTF8ToString($1));
        if (addTarget.value === "") return;
        var addValue = `<div class="todo"><p class="todoTitle">${addTarget.value}</p></div>`;
        var e = document.getElementById(UTF8ToString($0));
        e.innerHTML = e.innerHTML + addValue;
        addTarget.innerHTML = "";
    }, id.c_str(), id2.c_str());
}

extern "C" void EMSCRIPTEN_KEEPALIVE clicked4()
{
    setElementInnerHTMLToHTML("contents", "input1");
}
