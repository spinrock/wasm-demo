#include <stdio.h>
#include <string>
#include <emscripten.h>
#include <uuid/uuid.h>

int main() {
    printf("Hello, world\n");
    return 0;
}

extern "C" void EMSCRIPTEN_KEEPALIVE initialized()
{
    puts("initialized");
}

EM_JS(void *, getElementValue_, (char const *id),
{
    var e = document.getElementById(UTF8ToString(id));
    var str = e.value;
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

void setElementInnerHTML(std::string const &id, std::string const &html)
{
    EM_ASM({
        var e = document.getElementById(UTF8ToString($0));
        e.innerHTML = e.innerHTML + UTF8ToString($1);
    }, id.c_str(), html.c_str());
}

void setElementInnerHTMLfromInputForm(std::string const &id, std::string const &id2)
{
    std::string s = getElementValue(id2);
    if(s.empty()) return;

    uuid_t u0;
    uuid_generate(u0);
    char c0[16];
    uuid_unparse(u0, c0);
    printf("%s",c0);

    EM_ASM({
        var addValue = `<div id=${UTF8ToString($2)} class="todo"><p class="todoTitle">${UTF8ToString($1)}</p></div>`;
        var e = document.getElementById(UTF8ToString($0));
        e.innerHTML = e.innerHTML + addValue;
    }, id.c_str(), s.c_str(), c0);
}

extern "C" void EMSCRIPTEN_KEEPALIVE addTodo()
{
    setElementInnerHTMLfromInputForm("contents", "input1");
}
