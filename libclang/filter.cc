#include "filter.hh"
#include "data.hh"
#include "clang-c/Index.h"
#include <string>
#include <algorithm>

/** Creates a predicate function that returns true for every cursor
 * with a non-matching spelling.
 * */
auto const wrongName = [](std::string name)
{
  return [=name](CXCursor cursor)
  {
    CXString spelling = clang_getCursorSpelling(cursor);
    bool result = (name != clang_getCString(spelling));
    clang_disposeString(spelling);
    return result;
  }
};

/** Creates a predicate function that returns true for every cursor
 * that has a different type spelling (signature).
 *
 * The signature must match exactly, that is including whitespace.
 *
 * @todo Improve whitespace handling in signature string.
 * */
auto const wrongSignature = [](std::string signature)
{
  return [=signature](CXCursor cursor)
  {
    CXType type = clang_getCursorType(cursor);
    CXString spelling = clang_getTypeSpelling(type);
    bool result = (signature != clang_getCString(spelling));
    clang_disposeString(spelling);
    return result;
  }
};

/** Predicate function to filter out map pairs which don't match the
 * requested name and signature.
 *
 * Takes a value_type (a pair normally) of a map between declaration
 * cursors and declaration contents (@see declarations_map_type).
 * If the declarations methods contain no method matching the
 * given name and signature, then no code shall be generated for that
 * declaration.
 *
 * @warning This is highly impure, the method containers get
 * modified / emptied.
 *
 * @todo Is it possible that multiple methods match the name and
 * signature?
 * */
bool noCodeGenerationRequested(declarations_map_type::value_type & v)
{
  auto & methods = v.second.methods;
  std::remove_if(methods.begin(), methods.end(), wrongName("doit"));
  std::remove_if(methods.begin(), methods.end(),
      wrongSignature("void () const"));
  return (methods.size() != 1);
}
