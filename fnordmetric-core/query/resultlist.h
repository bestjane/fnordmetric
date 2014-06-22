/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * Licensed under the MIT license (see LICENSE).
 */
#ifndef _FNORDMETRIC_QUERY_RESULTLIST_H
#define _FNORDMETRIC_QUERY_RESULTLIST_H
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include "executable.h"

namespace fnordmetric {
namespace query {

class ResultList : public RowSink {
public:
  ResultList(const std::vector<std::string>& columns) :
      columns_(columns) {}

  ResultList(const ResultList& copy) = delete;
  ResultList& operator=(const ResultList& copy) = delete;

  void addRow(std::vector<std::unique_ptr<SValue>>&& row) override {
    rows_.emplace_back(std::move(row));
  }

  void debugPrint() const {
    std::vector<int> col_widths;
    int total_width = 0;

    for (const auto& col : columns_) {
      col_widths.push_back(20);
      total_width += 20;
    }

    auto print_hsep = [&col_widths] () {
      for (auto w : col_widths) {
        for (int i = 0; i < w; ++i) {
          char c = (i == 0 || i == w - 1) ? '+' : '-';
          printf("%c", c);
        }
      }
      printf("\n");
    };

    auto print_row = [this, &col_widths] (const std::vector<std::string>& row) {
      for (int n = 0; n < row.size(); ++n) {
        const auto& val = row[n];
        printf("| %s", val.c_str());
        for (int i = col_widths[n] - val.size() - 3; i > 0; --i) {
          printf(" ");
        }
      }
      printf("|\n");
    };

    print_hsep();
    print_row(columns_);
    print_hsep();
    for (const auto& row : rows_) {
      std::vector<std::string> str_row;
      for (const auto& value : row) {
        str_row.push_back(value->toString());
      }
      print_row(str_row);
    }
    print_hsep();
  }

protected:
  std::vector<std::string> columns_;
  std::vector<std::vector<std::unique_ptr<SValue>>> rows_;
};

}
}
#endif