#include <iostream>
#include <vector>
#include <memory>
 
template<typename T>
struct List {
  List() = default;
  List(List const&) = default;
  List(List&&) = default;
  virtual ~List() = default;

  List& operator=(List const&) = default;
  List& operator=(List&&) = default;
 
  inline void Append( T const * t ) { _list.push_back( t ); }
  typedef typename std::vector<T const *>::const_iterator  const_iterator;
 
  inline const_iterator cbegin() const { return _list.cbegin(); }
  inline const_iterator cend() const { return _list.cend(); }
 
private:
  std::vector< T const * > _list;
}; // struct List

struct Statement {
  virtual void Analyze() const = 0;
  
  virtual ~Statement() = default;
};
 
struct YetAnotherStatement: Statement {
  inline void Analyze() const final { std::cout << t << std::endl; }
  YetAnotherStatement( int i ): t{ ( double ) i * ( 10.6 / 0.7 ) } {}
  double t;
};
 
struct OtherStatement: Statement {
  inline void Analyze() const final { std::cout << t << std::endl; }
  OtherStatement( int i ): t{ i } {}
  int t;
};
 
struct CompoundStatement: Statement, List<Statement> {
  CompoundStatement() = default;
  CompoundStatement(CompoundStatement&&) = default; 

  CompoundStatement(CompoundStatement const&) = delete; 
  CompoundStatement& operator=(CompoundStatement const&) = delete;
  CompoundStatement& operator=(CompoundStatement&&) = delete;
  
  ~CompoundStatement(){
    for( auto b = cbegin(), d = cend(); b != d; ++b ) delete const_cast<Statement *>( *b );
  }
  void Analyze() const final {
    for( auto b = this->cbegin(); b != this->cend(); ++b ){
      (*b)->Analyze();
    }
  }
};
 
struct Declaration {
  Declaration( Statement const * const s ): s_( s ){}
  inline void Analyze(){ s_->Analyze(); }
  Statement const * const s_;
};
 
int main() {
  auto s = std::make_unique<CompoundStatement>();
  for( int i = 1; i <= 10; ++i ){
    if( i % 2 == 0 ) s->Append( new OtherStatement( i ) );
    else s->Append( new YetAnotherStatement( i ) );
  }
  Statement const * const p_s = s.get();
  Declaration d( p_s );
  d.Analyze();

  return 0;
}