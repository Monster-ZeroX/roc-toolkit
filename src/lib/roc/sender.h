/*
 * Copyright (c) 2015 Mikhail Baranov
 * Copyright (c) 2015 Victor Gaydov
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @brief Roc sender.

#ifndef ROC_SENDER_H_
#define ROC_SENDER_H_

#include "roc/config.h"
#include "roc/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct roc_sender roc_sender;

ROC_API roc_sender* roc_sender_new(const roc_config* config);

ROC_API void roc_sender_delete(roc_sender* sender);

ROC_API bool roc_sender_bind(roc_sender* sender, const char* address);

ROC_API ssize_t
roc_sender_write(roc_sender* sender, const float* samples, const size_t n_samples);

#ifdef __cplusplus
}
#endif

#endif // ROC_SENDER_H_