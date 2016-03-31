#include <iostream>
#include <vector>
#include <memory>
 
struct Statement {
  virtual void Analyze() const = 0;
  
  virtual ~Statement() = default;
};
 
struct YetAnotherStatement: Statement {
  void Analyze() const final {
    std::cout << t << std::endl;
  }
  YetAnotherStatement( int i ): t{ ( double ) i * ( 10.6 / 0.7 ) } {}
private:  
  double t;
};
 
struct OtherStatement: Statement {
  void Analyze() const final {
    std::cout << t << std::endl;
  }
  OtherStatement( int i ): t{ i } {}
private:  
  int t;
};
 
struct CompoundStatement: Statement {
  CompoundStatement() = default;
  CompoundStatement(CompoundStatement&&) = default; 

  CompoundStatement(CompoundStatement const&) = delete; 
  CompoundStatement& operator=(CompoundStatement const&) = delete;
  CompoundStatement& operator=(CompoundStatement&&) = delete;
  
  ~CompoundStatement(){
    for ( auto&& b : _statements ) {
      delete const_cast<Statement *>( b );
    }
  }
  
  void Analyze() const final {
    for ( auto&& b : _statements ) {
      b->Analyze();
    }
  }
  
  void Append(Statement const* statement) {
    _statements.push_back(statement);
  }

private:
  std::vector<Statement const*> _statements;
};
 
struct Declaration {
  Declaration( Statement const * const s ): s_( s ){}
  void Analyze() {
    s_->Analyze();
  }
private:  
  Statement const * const s_;
};
 
int main() {
  auto s = std::make_unique<CompoundStatement>();
  for ( int i = 1; i <= 10; ++i ) {
    if( i % 2 == 0 ) {
      s->Append( new OtherStatement( i ) );
    } else {
      s->Append( new YetAnotherStatement( i ) );
    }
  }
  Statement const * const p_s = s.get();
  Declaration d( p_s );
  d.Analyze();

  return 0;
}