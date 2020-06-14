#include <stdio.h>
#include <stdlib.h> // exit ()
#include <stdarg.h>
#include <stack>

#include "ast.h"
#include "symtab.h"

/*    This stack is used to implement  continue statements.
"continue"  is implemented as a goto to the label attached
to the code for the condition of the most closely enclosing while
statement.
This label is at the top of the stack. Below it on the stack is the
continue label of the enclosing while statement and so on.
The stack is empty when we are not inside a  while statement.
*/
static std::stack<int> continuelabels;
static std::stack<int> tempLabels;
static std::stack<int> exitlabels;

const int NO_CONTINUE = -1;
int label1;
int currentContLabel = NO_CONTINUE;
char currentResult[100];
int currentTemp;
bool isRepeat = false;
bool isLabel1Set = false;

//TODO refactor. Go over emits and prints

static
Object newTemp()
{
  static int counter = 1;
  char name[100];
  currentTemp = counter;
  sprintf(name, "_t%d", counter++);
  return Object(name);
}

#if 0
static
int newTemp (bool useThis)
{
  static int counter = 1;
  return counter++;
}
#endif

// labels are represented by numbers. For example, 3 means label3
static
int newlabel ()
{
  static int counter = 1;
  return counter++;
}

// emit works just like  printf  --  we use emit
// to generate code and print it to the standard output.
void emit (const char *format, ...)
{
  // if (errors > 0) return; // do not generate code if there are errors.  This should be controlled by if !defined (DEBUG))

  printf ("    ");  // this is meant to add a nice indentation.
  // Use emitlabel() to print a label without the indentation.
  va_list argptr;
  va_start (argptr, format);
  // all the arguments following 'format' are passed on to vprintf
  vprintf (format, argptr);
  va_end (argptr);
}

/* use this  to emit a label without using indentation */
void emitlabel (int label)
{
  printf ("label%d:\n",  label);
}

/* there are two versions of each arithmetic operator in the
generated code. One is  used for  operands having type int.
The other is used for operands having type float.
*/
struct operator_names {
  const char *int_name;
  const char *float_name;
};

static
struct operator_names
opNames [] = { {"+", "<+>"},
{"-", "<->"},
{"*", "<*>"},
{"/", "</>"}};

/* convert operator  to  string  suitable for the given type
e.g. opName (PLUS, _INT)  returns "+"
opName (PLUS, _FLOAT) returns  "<+>"
*/
const char *
opName (enum op op, myType t)
{
  if (op > DIV) { fprintf (stderr, "internal compiler error #1"); exit (1); }
  if (t == _INT)
  return opNames [op].int_name;
  else
  return opNames [op].float_name;
}

Object BinaryOp::genExp ()
{
  printf("Entered BinaryOp");
  Object left_operand_result = _left->genExp ();
  Object right_operand_result = _right->genExp ();
  Object result;


  char rightStr[100] = "_t";
  char rightTempIdStr[100];

  if(_op == XOR_OP){
    printf("\n XOR_OP\n");
    char result_xor_op[100];
    char temp_op[] = "^";
    // int str_len_tmp =
    strcpy(result_xor_op, left_operand_result._string);
    strncat(result_xor_op, temp_op, 1);
    strncat(result_xor_op, right_operand_result._string, strlen(right_operand_result._string));
    result = Object(result_xor_op);
  }
  else{
  if (_left->_type != _right->_type){
      printf("BinaryOp different types");
    Object temp = newTemp();
    int rightTempId = currentTemp;
    sprintf(rightTempIdStr, "%d", rightTempId);
    strncat(rightStr, rightTempIdStr , 1);

    switch (_left->_type ) {
      case _INT:
      emit(" %s = (int) %s\n", rightStr, right_operand_result._string);

      break;
      case _FLOAT:
      emit(" %s = (float) %s\n", rightStr, right_operand_result._string);
      break;
      default:
      fprintf (stderr, "internal compiler error #3\n"); exit (1);
    }
  }
  else{
    strcpy(rightStr, right_operand_result._string);
  }

  result = newTemp ();

  const char *the_op = opName (_op, _type);

  emit ("%s = %s %s %s BinaryOp::genExp\n", result._string, left_operand_result._string,
  the_op, rightStr);
  printf("\n");
  }
  return result;
}


Object NumNode::genExp ()
{
  // printf("inside NumNode::genExp\n");
  return (_type == _INT) ? Object(_u.ival) : Object(_u.fval);
  #if 0
  int result = newTemp ();
  if (_type == _INT)
  emit ("_t%d = %d int NumNode::genExp\n", result, _u.ival);
  else
  emit ("_t%d = %.2f flot NumNode::genExp\n", result, _u.fval);
  return result;
  #endif
}

Object IdNode::genExp()
{
  // printf("Inside IdNode::genExp()\n");
  return Object(_name);
  #if 0
  int result = newTemp ();

  emit ("_t%d = %s\n", result, _name);
  return result;
  #endif
}

void SimpleBoolExp::genBoolExp (int truelabel, int falselabel)
{
  if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
  return; // no need for code

  const char *the_op;


  Object left_result = _left->genExp ();
  Object right_result = _right->genExp ();

  strcpy(currentResult, left_result._string);


  if(isRepeat == true && isLabel1Set == false){ // TODO change to more elegant solution
    emitlabel(label1);
    isLabel1Set = true;
    isRepeat = false;
    // BinaryOp (enum op op, Exp *left, Exp *right, int line);
  }

  switch (_op) {
    case LT:
    the_op = "<";
    break;
    case GT:
    the_op = ">";
    break;
    case LE:
    the_op = "<=";
    break;
    case GE:
    the_op = ">=";
    break;
    case EQ:
    the_op = "==";
    break;
    case NE:
    the_op = "!=";
    break;
    default:
    fprintf (stderr, "internal compiler error #3\n"); exit (1);
  }

  if  (truelabel == FALL_THROUGH)
  emit ("ifFalse %s %s %s goto label%d\n", left_result._string, the_op,
  right_result._string, falselabel);
  else if (falselabel == FALL_THROUGH)
  emit ("if %s %s %s goto label%d\n", left_result._string, the_op,
  right_result._string, truelabel);
  else { // no fall through
    emit ("if %s %s %s goto label%d\n", left_result._string, the_op,
    right_result._string, truelabel);
    emit ("goto label%d\n", falselabel);
  }
}

void Or::genBoolExp (int truelabel, int falselabel)
{
  if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
  return; // no need for code

  if  (truelabel == FALL_THROUGH) {
    int next_label = newlabel(); // FALL_THROUGH implemented by jumping to next_label
    _left->genBoolExp (next_label, // if left operand is true then the OR expression
      //is true so jump to next_label (thus falling through
      // to the code following the code for the OR expression)
      FALL_THROUGH); // if left operand is false then
      // fall through and evaluate right operand
      _right->genBoolExp (FALL_THROUGH, falselabel);
      emitlabel (next_label);
    }  else if (falselabel == FALL_THROUGH) {
      _left->genBoolExp (truelabel, // if left operand is true then the OR expresson is true
        // so jump to  truelabel (without evaluating right operand)
        FALL_THROUGH); // if left operand is false then
        // fall through and evaluate right operand
        _right->genBoolExp (truelabel, FALL_THROUGH);
      } else { // no fall through
        _left->genBoolExp (truelabel, // if left operand is true then the or expresson is true
          // so jump to  truelabel (without evaluating right operand)
          FALL_THROUGH); // if left operand is false then
          // fall through and evaluate right operand
          _right->genBoolExp (truelabel, falselabel);
        }
}

void And::genBoolExp (int truelabel, int falselabel)
{
        if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
        return; // no need for code

        if  (truelabel == FALL_THROUGH) {
          _left->genBoolExp (FALL_THROUGH, // if left operand is true then fall through and evaluate
            // right operand.
            falselabel); // if left operand is false then the AND expression is
            // false so jump to falselabel);
            _right->genBoolExp (FALL_THROUGH, falselabel);
          } else if (falselabel == FALL_THROUGH) {
            int next_label = newlabel(); // FALL_THROUGH implemented by jumping to next_label
            _left->genBoolExp (FALL_THROUGH, // if left operand is true then fall through and
              // evaluate right operand
              next_label); // if left operand is false then the AND expression
              //  is false so jump to next_label (thus falling through to
              // the code following the code for the AND expression)
              _right->genBoolExp (truelabel, FALL_THROUGH);
              emitlabel(next_label);
            } else { // no fall through
              _left->genBoolExp (FALL_THROUGH, 	// if left operand is true then fall through and
                // evaluate right operand
                falselabel); // if left operand is false then the AND expression is false
                // so jump to falselabel (without evaluating the right operand)
                _right->genBoolExp (truelabel, falselabel);
              }
}

            void Nand::genBoolExp (int truelabel, int falselabel)
            {
              int next_label = newlabel();
              if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
              return; // no need for code

              if  (truelabel == FALL_THROUGH) {
                _left->genBoolExp (FALL_THROUGH,next_label);
                _right->genBoolExp (falselabel, FALL_THROUGH);
                emitlabel(next_label);
              } else if (falselabel == FALL_THROUGH) {
                _left->genBoolExp (FALL_THROUGH, next_label);
                _right->genBoolExp (falselabel, FALL_THROUGH);
                emitlabel(next_label);
              } else { // no fall through
                _left->genBoolExp (FALL_THROUGH, truelabel);

                _right->genBoolExp (falselabel, truelabel);
              }
            }

            void Not::genBoolExp (int truelabel, int falselabel)
            {
              _operand->genBoolExp (falselabel, truelabel);
            }

            void ReadStmt::genStmt()
            {
              myType idtype = _id->_type;

              if (idtype == _INT)
              emit ("iread %s\n", _id->_name);
              else
              emit ("fread %s\n", _id->_name);
            }

            void AssignStmt::genStmt()
            {

              Object result = _rhs->genExp();

              myType idtype = _lhs->_type;

              if (idtype == _rhs->_type)
              {
                printf("idtype == _rhs->_type");
                emit ("%s = %s assign stmt\n", _lhs->_name, result._string);
              }
              else
              {
                if (_lhs->_type != _rhs->_type && _lhs->_type != UNKNOWN)
                {
                  //       errorMsg ("line %d: left hand side and right hand side\
                  // of assignment have different types\n", _line);
                  switch (_lhs->_type ) {
                    case _INT:
                    _rhs->_type = _INT;
                    emit(" %s = (int) %s  ", _lhs->_name, result._string);
                    printf("!!!!Warning casting from float to int. Valuable data may be lost\n");
                    break;
                    case _FLOAT:
                    _rhs->_type = _FLOAT;
                    emit(" %s = (float) %s\n", _lhs->_name, result._string);
                    break;
                    default:
                    fprintf (stderr, "internal compiler error #3\n"); exit (1);
                  }
                }
              }
            }

            void AssignIota::genStmt()
            {

              Object result = _rhs->genExp();

              myType idtype = _lhs->_type;

              if (idtype == _rhs->_type)
              {
                printf("idtype == _rhs->_type");
                emit ("%s = %s assign stmt\n", _lhs->_name, result._string);
              }
              else
              {
                if (_lhs->_type != _rhs->_type && _lhs->_type != UNKNOWN)
                {
                  switch (_lhs->_type ) {
                    case _INT:
                    _rhs->_type = _INT;
                    emit(" %s = (int) %s  ", _lhs->_name, result._string);
                    printf("!!!!Warning casting from float to int. Valuable data may be lost\n");
                    break;
                    case _FLOAT:
                    _rhs->_type = _FLOAT;
                    emit(" %s = (float) %s\n", _lhs->_name, result._string);
                    break;
                    default:
                    fprintf (stderr, "internal compiler error #3\n"); exit (1);
                  }
                }
              }
            }

            

        


            void WhileStmt::genStmt()
            {
              int condlabel = newlabel ();
              int exitlabel = newlabel ();

              emitlabel(condlabel);
              currentContLabel = condlabel;
              _condition->genBoolExp (FALL_THROUGH, exitlabel);

              _body->genStmt ();



              emit ("goto label%d\n", condlabel);
              emitlabel(exitlabel);
            }

            void ContinueStmt::genStmt(){
              // continuelabels.push(currentContLabel);
              emit("goto label%d !!!!continue!!!\n", currentContLabel);
            }

            void IfStmt::genStmt()
            {
              int elseStmtlabel = newlabel ();
              int exitlabel = newlabel ();

              _condition->genBoolExp (FALL_THROUGH, elseStmtlabel);

              _thenStmt->genStmt ();
              emit ("goto label%d\n", exitlabel);
              emitlabel(elseStmtlabel);
              _elseStmt->genStmt();
              emitlabel(exitlabel);
            }

            void RepeatStmt::genStmt(){
              int condlabel = newlabel();
              int exitlabel = newlabel();

              // emitlabel(condlabel);
              label1 = condlabel;
              isRepeat = true;

              _condition->genBoolExp (exitlabel, FALL_THROUGH);

              _body->genStmt ();
              emit("%s = %s - 1\n", currentResult, currentResult);
              emit ("goto label%d\n", condlabel);
              emitlabel(exitlabel);
            }

            void Block::genStmt()
            {
              for (Stmt *stmt = _stmtlist; stmt != NULL; stmt = stmt->_next)
              stmt->genStmt();
            }

            void SwitchStmt::genStmt()
            {
              // int check_cases = newlabel();
              int default_stmt_label = 0, exitlabel = 0;
              Object temp = _exp->genExp();
              int result = currentTemp;

              // exitlabels.push(exitlabel);
              BreakStmt *case_break = new BreakStmt(_line);
              if (_exp->_type == _INT)
              {
                // emit("goto label%d\n", check_cases);
                Case* currect_case = _caselist;
                Case* temp_current_case = _caselist;


                while (temp_current_case != NULL){
                  temp_current_case->_label = newlabel();
                  tempLabels.push(temp_current_case->_label);
                  emit("if _t%d == %d goto label%d\n",result, temp_current_case->_number,temp_current_case->_label);
                  // if (currect_case->_hasBreak)
                  // case_break->genStmt();
                  temp_current_case = temp_current_case->_next;
                }
                default_stmt_label = newlabel();
                exitlabel = newlabel();
                tempLabels.push(default_stmt_label);
                tempLabels.push(exitlabel);
                emit("goto label%d\n", default_stmt_label);

                while(!tempLabels.empty()){
                  exitlabels.push(tempLabels.top());
                  tempLabels.pop();
                }

                while (currect_case != NULL)
                {
                  // currect_case->_label = newlabel();
                  // emitlabel(currect_case->_label);
                  emitlabel(exitlabels.top());
                  exitlabels.pop();
                  currect_case->_stmt->genStmt();

                  if (currect_case->_hasBreak){
                    case_break->genStmt();
                  }
                  currect_case = currect_case->_next;
                  emit("goto label%d\n", exitlabel);
                }

                emitlabel(default_stmt_label);
                _default_stmt->genStmt();
                case_break->genStmt();

                currect_case = _caselist;
              }
              else{
                errorMsg ("on line %d expression inside switch is not of type int\n", _line);
              }
              exitlabels.pop();
              emitlabel(exitlabel);
            }

            void BreakStmt::genStmt()
            {
              int isEmpty = exitlabels.empty();
              if(isEmpty){
                errorMsg ("line %d. Break not in loop or switch case\n", _line);
              }
            }
