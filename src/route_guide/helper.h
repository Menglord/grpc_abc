/*
 * helper.h
 *
 *  Created on: Dec 17, 2018
 *      Author: meng
 */

#ifndef SRC_ROUTE_GUIDE_HELPER_H_
#define SRC_ROUTE_GUIDE_HELPER_H_

#include <string>
#include <vector>

namespace routeguide {
class Feature;

std::string GetDbFileContent(int argc, char** argv);

void ParseDb(const std::string& db, std::vector<Feature>* feature_list);

} /* namespace routeguide */

#endif /* SRC_ROUTE_GUIDE_HELPER_H_ */
