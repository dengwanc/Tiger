#include "../ast/index.h"
#include "../utils/index.h"

namespace ast {
char *Lvalue::stringify() {
  return String("Lvalue");
}

char *Expr::stringify() {
  return String("Expr");
}
}