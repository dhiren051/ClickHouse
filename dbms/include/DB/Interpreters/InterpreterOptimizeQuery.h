#pragma once

#include <DB/Storages/IStorage.h>
#include <DB/Parsers/ASTOptimizeQuery.h>
#include <DB/Interpreters/Context.h>


namespace DB
{


/** Просто вызвать метод optimize у таблицы.
  */
class InterpreterOptimizeQuery
{
public:
	InterpreterOptimizeQuery(ASTPtr query_ptr_, Context & context_)
		: query_ptr(query_ptr_), context(context_) {}

	void execute()
	{
		const ASTOptimizeQuery & ast = dynamic_cast<const ASTOptimizeQuery &>(*query_ptr);
		context.getTable(ast.database, ast.table)->optimize();
	}

private:
	ASTPtr query_ptr;
	Context context;
};


}
