#include <iostream>
#include <stdio.h>
#include <string>
#include <utility>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std;

#include "httplib/httplib.h"

std::string fetch (const std::string& url) {

    std::string domain = "";
    std::string path = "";
    if (url.rfind("https://", 0) == 0) {
        domain = url.substr(8, url.length()-8);
    }
    else if (url.rfind("http://", 0) == 0) {
        domain = url.substr(7, url.length()-7);
    } else {
        domain = url;
    }

    int firstSlash = domain.find("/");

    for (int i = firstSlash; i < domain.length(); i++)
        path += domain.at(i);

    for (int i = firstSlash; i < domain.length(); i++)
        domain.erase(firstSlash);

    if (path.empty()) path = "/";

    cout << domain << " | " << path << endl;

    httplib::Client cli(domain, 80);

    auto res = cli.Get(path.c_str());

    if (res) return res->body;
    else     return std::to_string(static_cast<double>(res.error()));
}

//https://stackoverflow.com/questions/66122109/how-to-check-if-any-key-is-pressed-in-pixel-game-engine
struct KeyCharMap{
    olc::Key key;
    std::string lower;
    std::string upper;
};
std::vector<KeyCharMap> valueInputKeys = {
    {olc::A, "a", "A"},
    {olc::B, "b", "B"},
    {olc::C, "c", "C"},
    {olc::D, "d", "D"},
    {olc::E, "e", "E"},
    {olc::F, "f", "F"},
    {olc::G, "g", "G"},
    {olc::H, "h", "H"},
    {olc::I, "i", "I"},
    {olc::J, "j", "J"},
    {olc::K, "k", "K"},
    {olc::L, "l", "L"},
    {olc::M, "m", "M"},
    {olc::N, "n", "N"},
    {olc::O, "o", "O"},
    {olc::P, "p", "P"},
    {olc::Q, "q", "Q"},
    {olc::R, "r", "R"},
    {olc::S, "s", "S"},
    {olc::T, "t", "T"},
    {olc::U, "u", "U"},
    {olc::V, "v", "V"},
    {olc::W, "w", "W"},
    {olc::X, "x", "X"},
    {olc::Y, "y", "Y"},
    {olc::Z, "z", "Z"},
    {olc::K0, "0", "0"},
    {olc::K1, "1", "!"},
    {olc::K2, "2", "@"},
    {olc::K3, "3", "#"},
    {olc::K4, "4", "$"},
    {olc::K5, "5", "%"},
    {olc::K6, "6", "^"},
    {olc::K7, "7", "&"},
    {olc::K8, "8", "*"},
    {olc::K9, "9", "("},
    {olc::NP0, "0", "0"},
    {olc::NP1, "1", "1"},
    {olc::NP2, "2", "2"},
    {olc::NP3, "3", "3"},
    {olc::NP4, "4", "4"},
    {olc::NP5, "5", "5"},
    {olc::NP6, "6", "6"},
    {olc::NP7, "7", "7"},
    {olc::NP8, "8", "8"},
    {olc::NP9, "9", "9"},
    {olc::NP_MUL, "*", "*"},
    {olc::NP_DIV, "/", "/"},
    {olc::NP_ADD, "+", "+"},
    {olc::NP_SUB, "-", "-"},
    {olc::NP_DECIMAL, ".", "."},
    {olc::PERIOD, ".", ">"},
    {olc::SPACE, " ", " "},
    {olc::OEM_1, ";", ":"},
    {olc::OEM_2, "/", "?"},
    {olc::OEM_3, "`", "~"},
    {olc::OEM_4, "[", "{"},
    {olc::OEM_5, "\\", "|"},
    {olc::OEM_6, "]", "}"},
    {olc::OEM_7, "'", "\""},
    {olc::OEM_8, "-", "-"},
    {olc::EQUALS, "=", "+"},
    {olc::COMMA, ",", "<"},
    {olc::MINUS,"-","_"}
};


class HitBox {
public:
    int x;
    int y;
    int w;
    int h;

    HitBox (int x_, int y_, int w_, int h_) {
        x = x_;
        y = y_;
        w = w_;
        h = h_;
    }

    bool CollidingWith (int x_, int y_) {
        return (
            x_ > x &&
            x_ < x + w &&
            y_ > y &&
            y_ < y + h
        );
    }
};

class sprite {
public:
    sprite () {}

    virtual void draw (olc::PixelGameEngine* instance){
        cout << "draw not overridden" << endl;
    };
};

class textBox : public sprite {
private:
    int x;
    int y;
    int xSize;
    int ySize;
    std::string text;
    olc::Pixel boxColour;
    olc::Pixel boxColourHovered;
    olc::Pixel textColour;
    int fontSize;
    bool hovered;
    bool focused;
    int cursorPos;

public:
    std::function<void(textBox*)> onSubmit;

    textBox (int x_, int y_, int xSize_, int ySize_, std::string defaultText, olc::Pixel boxColour_, olc::Pixel boxColourHovered_, olc::Pixel textColour_, int fontSize_ = 1, std::function<void(textBox*)> onSubmit_ = [](textBox* box){})
            : sprite()
    {
        x = x_;
        y = y_;
        xSize = xSize_;
        ySize = ySize_;
        text = defaultText;
        boxColour = boxColour_;
        boxColourHovered = boxColourHovered_;
        textColour = textColour_;
        fontSize = fontSize_;
        hovered = false;
        focused = false;
        cursorPos = 0;
        onSubmit = onSubmit_;
    }

    void checkInput (olc::PixelGameEngine *instance) {
        if (instance->GetKey(olc::Key::LEFT).bPressed) {
            if (cursorPos > 0)
                cursorPos--;
        }
        if (instance->GetKey(olc::Key::RIGHT).bPressed) {
            if (cursorPos < text.length())
                cursorPos++;
        }

        if (instance->GetKey(olc::Key::BACK).bPressed) {
            if (text.length() > 0) {
                text.erase(cursorPos-1);
                if (cursorPos > text.length())
                    cursorPos--;
            }
        }

        if (instance->GetKey(olc::Key::ENTER).bPressed) {
            onSubmit(this);
        }

        for (auto& m : valueInputKeys) {
            if (instance->GetKey(m.key).bPressed) {
                std::string add = m.lower;
                if (instance->GetKey(olc::Key::SHIFT).bHeld)
                    add = m.upper;

                text.insert(cursorPos, add);
                cursorPos++;
            }
        }

    }

    void draw (olc::PixelGameEngine *instance) {
        HitBox hitBox (x - xSize / 2, y - ySize / 2, xSize, ySize);

        hovered = hitBox.CollidingWith(instance->GetMouseX(), instance->GetMouseY());


        if (hovered)
            instance->FillRect(hitBox.x, hitBox.y, xSize, ySize, boxColourHovered);
        else
            instance->FillRect(hitBox.x, hitBox.y, xSize, ySize, boxColour);


        if (instance->GetMouse(0).bReleased) {
            if (hovered) {
                focused = true;
                cursorPos = text.length();

            } else
                focused = false;
        }

        if (focused)
            checkInput(instance);

        instance->DrawString(x - text.length() * 8, y, text, textColour, fontSize);

    }

    std::string val () {
        return text;
    }

    std::string val (std::string str) {
        text = str;
        return text;
    }
};

class text : public sprite {
private:
    int x;
    int y;
    olc::Pixel textColour;
    int fontSize;
    std::string text_;

public:

    text (int x_, int y_, std::string defaultText, olc::Pixel textColour_, int fontSize_ = 1)
            : sprite()
    {
        x = x_;
        y = y_;
        text_ = defaultText;
        textColour = textColour_;
        fontSize = fontSize_;
    }


    void draw (olc::PixelGameEngine *instance) {
        instance->DrawString(x, y, text_, textColour, fontSize);
    }

    void setText (std::string val) {
        text_ = val;
    }
};

class OLCWindow : public olc::PixelGameEngine {
private:
    std::vector<sprite*> sprites;

public:
    OLCWindow () {
        sAppName = "WebBrowser";
    }

    bool OnUserCreate () override {


        auto* result = new text(5, 50, fetch("www.google.com"), olc::BLACK);

        auto* searchBox = new textBox(ScreenWidth() / 2, 10, 500, 30, "www.google.com", olc::BLACK, olc::GREY, olc::WHITE, 2, [=](textBox* box) {
            cout << box->val() << endl;
            std::string res = fetch(box->val());
            result->setText(res);
        });

        sprites = {searchBox, result};
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame

        // clear screen
        FillRect(0 , 0, ScreenWidth(), ScreenHeight());

        // draw all sprites
        for (sprite* sprite_ : sprites) {
            sprite_->draw(this);
        }

        return true;
    }
};

int main() {
    OLCWindow demo;
    if (demo.Construct(1000, 500, 1, 1))
        demo.Start();
    return 0;
}