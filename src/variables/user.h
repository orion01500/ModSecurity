/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>

#ifndef SRC_VARIABLES_USER_H_
#define SRC_VARIABLES_USER_H_

#include "src/variables/variable.h"
#include "src/run_time_string.h"

namespace modsecurity {

class Transaction;
namespace Variables {


class User_DictElement : public Variable {
 public:
    explicit User_DictElement(std::string dictElement)
        : Variable("USER"),
        m_dictElement("USER:" + dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_collections.resolveMultiMatches(m_dictElement,
            "USER", transaction->m_rules->m_secWebAppId.m_value, l);
    }

    std::string m_dictElement;
};


class User_NoDictElement : public Variable {
 public:
    User_NoDictElement()
        : Variable("USER") { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_collections.resolveMultiMatches(m_name, "USER",
            transaction->m_rules->m_secWebAppId.m_value, l);
    }
};


class User_DictElementRegexp : public Variable {
 public:
    explicit User_DictElementRegexp(std::string dictElement)
        : Variable("USER"),
        m_r(dictElement),
        m_dictElement("USER:" + dictElement) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        transaction->m_collections.resolveRegularExpression(m_dictElement,
            "USER", transaction->m_rules->m_secWebAppId.m_value, l);
    }

    Utils::Regex m_r;
    std::string m_dictElement;
};


class User_DynamicElement : public Variable {
 public:
    explicit User_DynamicElement(std::unique_ptr<RunTimeString> dictElement)
        : Variable("USER:dynamic"),
        m_string(std::move(dictElement)) { }

    void evaluate(Transaction *transaction,
        Rule *rule,
        std::vector<const collection::Variable *> *l) override {
        std::string string = m_string->evaluate(transaction);
        transaction->m_collections.resolveMultiMatches("USER:" + string, "USER", l);
    }

    std::unique_ptr<RunTimeString> m_string;
};


}  // namespace Variables
}  // namespace modsecurity

#endif  // SRC_VARIABLES_USER_H_
