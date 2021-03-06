/*  Copyright (c) 2013, Abdullah A. Hassan <voodooattack@hotmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "renderer.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <locale>

namespace ADWIF
{
  std::string colourStr(Colour col)
  {
    switch(col)
    {
      case Colour::Black: return "Black";
      case Colour::Blue: return "Blue";
      case Colour::Cyan: return "Cyan";
      case Colour::Green: return "Green";
      case Colour::Magenta: return "Magenta";
      case Colour::Red: return "Red";
      case Colour::White: return "White";
      case Colour::Yellow: return "Yellow";
      case Colour::Default: return "Default";
    }
    return "Default";
  }

  Colour strColour(const std::string & colour)
  {
    std::string c;
    std::transform(colour.begin(), colour.end(), std::back_inserter(c), &tolower);
    if (c == "black")
      return Colour::Black;
    else if (c == "blue")
      return Colour::Blue;
    else if (c == "cyan")
      return Colour::Cyan;
    else if (c == "green")
      return Colour::Green;
    else if (c == "magenta")
      return Colour::Magenta;
    else if (c == "red")
      return Colour::Red;
    else if (c == "white")
      return Colour::White;
    else if (c == "yellow")
      return Colour::Yellow;
    else
      return Colour::Default;
  }

  std::vector<std::string> styleStrs(int style)
  {
    std::vector<std::string> strs;
    if (style & Style::Bold)
      strs.push_back("Bold");
    if (style & Style::Underline)
      strs.push_back("Underline");
    if (style & Style::Dim)
      strs.push_back("Dim");
    if (style & Style::StandOut)
      strs.push_back("StandOut");
    if (style & Style::AltCharSet)
      strs.push_back("AltCharSet");
    if (!style || style & Style::Normal)
      strs.push_back("Normal");
    return strs;
  }

  int strsStyle(const std::vector<std::string> & strs)
  {
    int style = 0;
    for (auto s : strs)
    {
      std::transform(s.begin(), s.end(), s.begin(), &tolower);

      if (s == "bold")
        style |= Style::Bold;
      else if (s == "underline")
        style |= Style::Underline;
      else if (s == "dim")
        style |= Style::Dim;
      else if (s == "standout")
        style |= Style::StandOut;
      else if (s == "altcharset")
        style |= Style::AltCharSet;
      else
        style |= Style::Normal;
    }

    return style;
  }

}
