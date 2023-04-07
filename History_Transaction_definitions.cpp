#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){
  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;
  if(buy_sell_trans){
    trans_type = "Buy";
  } else {
    trans_type = "Sell";
  }
  shares = number_shares;
  amount = trans_amount;
  trans_id = assigned_trans_id;
  ++assigned_trans_id;
  acb = 0;
  cgl = 0.0;
  acb_per_share = 0.0;
  share_balance = 0;
}

// Destructor
// TASK 1
//
Transaction::~Transaction(){}

// Overloaded < operator.
// TASK 2
// The left operands time must be prior to the right operands for this function to return true
// If their dates are equal, than if the left operands transaction id is greater than the right operands, return true
//
bool Transaction::operator<(Transaction const &other)
{
  // 3 cases: the year after, the same, or before
  if ((this->get_year()) > (other.get_year())) {
    return false;
  } else if ((this->get_year()) == (other.get_year())) {
    // 3 cases: month is after, same, prior
    if ((this->get_month()) > (other.get_month())) {
      return false;
    } else if ((this->get_month()) == (other.get_month())) {
      // three cases, day is after, same, prior
      if ((this->get_day()) > (other.get_day())) {
        return false;
      } else if ((this->get_day()) == (other.get_day())) {
        if ((this->get_trans_id()) > (other.get_trans_id())) {
          return true;
        }
        assert( ((this->get_trans_id()) < (other.get_trans_id())) || ((this->get_trans_id()) == (other.get_trans_id())) );
        return false;
      } else {
        return true;
      }
    } else {
      return true;
    }
  } else {
    return true;
  }
}
// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 3
//
History::History(){
  p_head = nullptr;
}
// Destructor
// TASK 3
//
History::~History(){
  while(p_head != nullptr){
    //create a temporary object to store the old linked list head
    Transaction *p_old_head = p_head;
    //set the current head to the next node 
    p_head = (p_head)->get_next();
    //ensure no memory leaks
    delete p_old_head;
    p_old_head = nullptr;
  }  
}
// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
  ece150::open_file();
  //loop through the file while this member function call is true
  while(ece150::next_trans_entry()){
    //local variable pointing to a new transaction object
    Transaction *p_new_transaction = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type() , ece150::get_trans_shares(), ece150::get_trans_amount());
    insert(p_new_transaction);
  }
  ece150::close_file();
}
// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){
  //since this pointer will be the last node of the linked list
  p_new_trans->set_next(nullptr);
  //linked list is empty
  if(p_head == nullptr){
    p_head = p_new_trans;
  //linked list is filled
  } else {
    Transaction *p_transaction = p_head;
    //loop through the next transaction pointer untill we have reached the end (which will be nullptr)
    while((p_transaction->get_next()) != nullptr){
      p_transaction = p_transaction->get_next();
    }
    //set end of linked list to point to the parameter
    p_transaction->set_next(p_new_trans);
    //now the end of the linked list should be p_new_trans and it should point to nullptr
  }
}
// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){
  //cannot sort an empty list
  assert (p_head != nullptr);
  //local variables
  unsigned int size{0};
  unsigned int num;
  Transaction* p_1= p_head;
  Transaction* p_2;
  Transaction* p_tail;
  Transaction* p_temp;
  bool isSwapped;
  
  //loop to find the size of the linked list
  while( p_1->get_next() != nullptr ){
    ++size;
    p_1= p_1->get_next();
  }
  for(unsigned int i{0}; i < size; ++i){
    p_2 = p_head->get_next();
    p_1= p_2->get_next();
    p_tail = p_head;
    isSwapped = false;
    for(unsigned int k{0}; k < size - 1 - i; ++k){
      if(*p_2 < *p_head){
        p_tail->set_next(p_2->get_next());
        p_2->set_next(p_tail);
        p_temp = p_2;
        p_2 = p_2->get_next();
        //make sure the tail is first
        p_tail = p_temp;
        p_head = p_tail;
        isSwapped = true;
      } else if(*p_1 < *p_2){
        p_2->set_next(p_1->get_next());
        p_1->set_next(p_2);
        p_tail->set_next(p_1);
        isSwapped = true;
      }
      p_tail = p_tail->get_next();
      p_2 = p_tail->get_next();
      p_1= p_2->get_next();
    }
    if(!isSwapped) {
      break;
    }
  }
  /*
  //cannot sort an empty list
  assert (p_head != nullptr);
  //local variables
  unsigned int size{0};
  Transaction* p_1= p_head;
  Transaction* p_2;
  Transaction* p_tail;
  Transaction* p_temp;
  
  //loop to find the size of the linked list
  while( p_1 != nullptr ){
    ++size;
    p_1= p_1->get_next();
  }
  for(unsigned int i{0}; i < size; ++i){

    std::cout << "Started a walk through" << std::endl;

    p_2 = p_head->get_next();
    p_1= p_2->get_next();
    p_tail = p_head;

    std::cout << "p_tail:\n";
    p_tail->print();

    std::cout << "p_head:\n";
    p_head->print();

    std::cout << "p_2: \n";
    p_2->print();

    std::cout << "p_1: \n";
    p_1->print();
    for(unsigned int k{0}; k < size - i - 1; ++k){
      if(*p_2 < *p_head){

        std::cout << " Head is Greater" << std::endl;

        p_tail->set_next(p_2->get_next());
        p_2->set_next(p_tail);
        p_temp = p_2;
        p_2 = p_2->get_next();
        //make sure the tail is first
        p_tail = p_temp;
        p_head = p_tail;

        std::cout << "p_tail:\n";
        p_tail->print();

        std::cout << "p_2:\n";
        p_2->print();

        std::cout << "p_1:\n";
        p_1->print();

      } else if(*p_1 < *p_2){
        p_2->set_next(p_1->get_next());
        p_1->set_next(p_2);
        p_tail->set_next(p_1);
      }
      p_tail = p_tail->get_next();
      p_2 = p_tail->get_next();
      p_1= p_2->get_next();

      std::cout << "p_tail:\n";
      p_tail->print();

      std::cout << "p_2:\n";
      p_2->print();

      std::cout << "p_1:\n";
      p_1->print();

      std::cout << "Finished increment" << std::endl;
    }
    std::cout << "Reached end of inner loop" << std::endl;
  }
  */
  
  /*
  OLD BUBBLE SORT 2.0
  WHY DOESNT THIS WORK?
  
  Transaction* p_current;
  Transaction* p_next = p_current->get_next();
  Transaction* p_temp;
  for(unsigned int k{0}; k < size - 1; ++k){
    p_current = p_head;
    p_next = p_current->get_next();
    for(unsigned int h{0}; h < size - k - 1; ++h){
      if(*p_next < *p_current){
        p_temp = p_next;
        p_current->set_next(p_next->get_next());
        p_temp->set_next(p_current);
        if(p_current == p_head){
          p_head = p_temp;
        } else {
          p_current = p_temp;
        }
      } else {
        p_current = p_next;
      }
      p_current = p_next;
      p_next = p_current->get_next();
    }
  }
  */
 //std::cout << "finished sorting" << std::endl;
} 
// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
  //local variables, we need to keep track of the transactions from before
  Transaction* p_before = p_head;
  Transaction* p_current = p_head;
  //loop through whole list
  while (p_current != nullptr){
    //if this is the first transaction
    if(p_current == p_head){
      //for a buy
      if(p_current->get_trans_type()){
        p_current->set_acb(p_current->get_amount());
        p_current->set_share_balance(p_current->get_shares());
        p_current->set_acb_per_share( (p_current->get_acb()) / (p_current->get_share_balance()) );
        p_current->set_cgl(0.0);
      } else {
        p_current->set_acb(0.0);
        p_current->set_share_balance( -(p_current->get_shares()) );
        p_current->set_acb_per_share(0.0);
        p_current->set_cgl( -(p_current->get_amount()) );
      }
    //is not the first transaction
    } else {
      if(p_current->get_trans_type()){
        p_current->set_acb( p_before->get_acb() + p_current->get_amount());
        p_current->set_share_balance(p_current->get_shares() + p_before->get_share_balance());
        p_current->set_acb_per_share( (p_current->get_acb()) / (p_current->get_share_balance()) );        p_current->set_cgl(0.0);
      } else {
          p_current->set_acb(p_before->get_acb() - (p_current->get_shares() * p_before->get_acb_per_share()));
          p_current->set_share_balance(p_before->get_share_balance() - p_current->get_shares());
          p_current->set_acb_per_share( p_before->get_acb_per_share() );
          p_current->set_cgl( (p_current->get_amount()) - (p_current->get_shares() * p_before->get_acb_per_share()) );
      }
    }
    //store this because we need to sum previous details
    p_before = p_current;
    p_current = p_current->get_next();
  }
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year){
  double total_cgl = 0.0;
  Transaction* p_current = p_head;
  //loop through the whole History linked list
  while(p_current != nullptr){
    if(p_current->get_year() == year){
      total_cgl += p_current->get_cgl();
    }
    //always move to next transaction at the end
    p_current = p_current->get_next(); 
  }
  return total_cgl;
}
// print() Print the transaction history.
//TASK 9
//
void History::print(){
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  Transaction *p_transaction = p_head;
  while(p_transaction != nullptr){
    p_transaction->print();
    p_transaction = p_transaction->get_next();
  }
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}
// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
