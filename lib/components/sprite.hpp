#pragma once
#include "lib/object.h"
#include "lib/rapidxml/rapidxml.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"
#include <iostream>
#include <map>
#include <locale>
#include <codecvt>
#include <ncurses.h>
#include <cctype>
using namespace std;
using namespace rapidxml;

size_t utf8size(char* str) {
  size_t len = 0;
  while (*str) len += (*str++ & 0xc0) != 0x80;
  return len;
}

u32string to_utf32(const string &s) {
  wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
  return conv.from_bytes(s);
}

std::string to_utf8(const std::u32string &s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}

unsigned getChildCount(xml_node<> *n) {
  unsigned c = 0;
  for (xml_node<> *child = n->first_node(); child; child = child->next_sibling()) {
    c++;
  } 
  return c;
} 

struct Frame {
  u32string * chars;
  char** color;
  short height;
  short width;
  short pivotX = 0;
  short pivotY = 0;
};

struct Anim {
  Frame* frames;
  unsigned duration;
};

class Sprite : public Render {
 private:
  Transform* transform;
  map<string, Anim> animations;
  string anim = "default";
  size_t currentFrame = 0;
  bool active = false;
  string filename;

  inline int encode(char ch) {
    chtype color = 0;
    switch(tolower(ch)) {
      case 'b': return COLOR_BLACK;   break;
      case 'r': return COLOR_RED;     break;
      case 'g': return COLOR_GREEN;   break;
      case 'y': return COLOR_YELLOW;  break;
      case 'l': return COLOR_BLUE;    break;
      case 'p': return COLOR_MAGENTA; break;
      case 'c': return COLOR_CYAN;    break;
      case 'w': return COLOR_WHITE;   break;
    }
    color = NCURSES_BITS(1U, color);
    if(isupper(ch)) color |= A_BOLD; // Make bright
    return color;
  }

  void draw() {
    Frame& frame = animations[anim].frames[currentFrame];
    for(short i = 0; i < frame.height; ++i) {
      for(short j = 0; j < frame.width; ++j) {
        if(frame.color[i][j] == ' ') {
          continue;
        }
        if(i+transform->y()+1-frame.pivotY < 0 || j+transform->x()+1-frame.pivotX < 0) continue;
        move(i+transform->y()+1-frame.pivotY, j+transform->x()+1-frame.pivotX);
        string chars = to_utf8(u32string(1, frame.chars[i][j]));
        char ch = frame.color[i][j];
        int color = encode(ch);
        attron(COLOR_PAIR(color));
        if(islower(ch)) attron(A_DIM           );

        printw("%s", chars.c_str());

        attroff(COLOR_PAIR(color));
        if(islower(ch)) attroff(A_DIM           );
      }
    }
  }
 public:
  Sprite(string name) {
    filename = name;
  }

  void start() {
    file<char> original(filename.c_str());
    xml_document<char> doc;
    doc.parse<0>(original.data());

    xml_node<> *mainNode = doc.first_node("sprite");


    for(xml_node<> *animation = mainNode->first_node("animation"); animation; animation = animation->next_sibling("animation")) {
      // Parsing animation
      auto name = animation->first_attribute("name");
      if(!name) throw "Not named animation";

      Frame* frames;
      unsigned length = getChildCount(animation);
      frames = new Frame[length];
      unsigned frameIndex = 0;

      for(xml_node<> *frame = animation->first_node("frame"); frame; frame = frame->next_sibling("frame")) {
        // Parsing frame
        Frame current;
        current.height = getChildCount(frame);
        current.color = new char*[current.height];
        current.chars = new u32string[current.height];
        if(frame->first_attribute("pivotX")) current.pivotX = atoi(frame->first_attribute("pivotX")->value());
        if(frame->first_attribute("pivotY")) current.pivotY = atoi(frame->first_attribute("pivotY")->value());

        bool widthSet = false;
        unsigned width = 0;
        unsigned rowIndex = 0;

        for(xml_node<> *row = frame->first_node("row"); row; row = row->next_sibling("row")) {
          // Getting color information
          auto colorElement = row->first_attribute("color");
          if(!colorElement) throw "Missing color attribute for row";
          if(!widthSet) {
            width = colorElement->value_size();
            widthSet = true;
          }

          // A bit of error checking
          else if(colorElement->value_size() != width) throw "Sprite is not rectangle";

          current.color[rowIndex] = new char[width];
          copy(colorElement->value(),
              colorElement->value() + width,
              current.color[rowIndex]);
          
          
          // Getting chars
          auto chars = row->first_attribute("chars");

          if(!chars) throw "Missing chars attribute for row";
          if(utf8size(chars->value()) != width) throw "Chars attr is not equal length to color";

          current.chars[rowIndex] = to_utf32(string(chars->value(), chars->value_size()));

          ++rowIndex;
        }

        current.width = width;
        frames[frameIndex] = current;
        ++frameIndex;
      }

      animations[name->value()] = { frames, length };
    }

    transform = getComponent<Transform>(); // I ^o^ Unity
  }

  void startPlaying() { active = true; }
  void stopPlaying() { active = false; }
  void setTrack(string name) { anim = name; currentFrame = 0; }

  void update() {
    if(animations.find(anim) == animations.end()) return;
    if(active) {
      ++currentFrame;
      currentFrame %= animations[anim].duration;
    }
  }

};