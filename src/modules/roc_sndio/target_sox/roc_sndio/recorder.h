/*
 * Copyright (c) 2015 Mikhail Baranov
 * Copyright (c) 2015 Victor Gaydov
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//! @file roc_sndio/target_sox/roc_sndio/recorder.h
//! @brief Audio reader.

#ifndef ROC_SNDIO_RECORDER_H_
#define ROC_SNDIO_RECORDER_H_

#include <sox.h>

#include "roc_audio/iwriter.h"
#include "roc_core/atomic.h"
#include "roc_core/buffer_pool.h"
#include "roc_core/iallocator.h"
#include "roc_core/slice.h"
#include "roc_core/stddefs.h"
#include "roc_core/thread.h"
#include "roc_packet/units.h"

namespace roc {
namespace sndio {

//! Audio recorder.
//! @remarks
//!  Reads samples from input file or audio driver, decodes them and
//!  writes buffers to output writer.
class Recorder : private core::Thread {
public:
    //! Initialize.
    //!
    //! @b Parameters
    //!  - @p buffer_pool is used to allocate buffers;
    //!  - @p n_samples defines number of samples per channel in output buffers;
    //!  - @p channels defines bitmask of enabled channels in output buffers;
    //!  - @p sample_rate defines sample rate of output buffers.
    Recorder(core::BufferPool<audio::sample_t>& buffer_pool,
             packet::channel_mask_t channels,
             size_t n_samples,
             size_t sample_rate);

    virtual ~Recorder();

    //! Open input file or device.
    //!
    //! @b Parameters
    //!  - @p name is input file or device name, "-" for stdin.
    //!  - @p type is codec or driver name.
    //!
    //! @remarks
    //!  If @p name or @p type are NULL, they're autodetected.
    //!
    //! @pre
    //!  Should be called once before calling start().
    bool open(const char* name, const char* type);

    //! Get sample rate of an input file or a device.
    //!
    //! @pre
    //!  Input file or device should be opened.
    size_t sample_rate() const;

    //! Returns true if input is a real file.
    //!
    //! @pre
    //!  Input file or device should be opened.
    bool is_file() const;

    //! Start writing samples in a separate thread.
    //!
    //! @b Parameters
    //!  - @p output is used to write buffers with decoded samples.
    bool start(audio::IWriter& output);

    //! Stop thread.
    //! @remarks
    //!  Can be called from any thread.
    void stop();

    //! Wait until background thread finishes.
    //! @remarks
    //!  Should be called once.
    void join();

private:
    virtual void run();

    static const sox_effect_handler_t output_handler_;

    static int kill_cb_(sox_effect_t* eff);
    static int output_cb_(sox_effect_t* eff,
                          const sox_sample_t* ibuf,
                          sox_sample_t* obuf,
                          size_t* ibufsz,
                          size_t* obufsz);

    bool prepare_();
    bool open_(const char* name, const char* type);
    void write_(const sox_sample_t* buf, size_t bufsz, bool eof);
    void close_();

    sox_signalinfo_t out_signal_;

    sox_format_t* input_;
    sox_effects_chain_t* chain_;

    audio::IWriter* output_;
    core::BufferPool<audio::sample_t>& buffer_pool_;

    core::Slice<audio::sample_t> buffer_;
    size_t buffer_pos_;

    size_t buffer_size_;
    size_t n_bufs_;
    bool is_file_;

    core::Atomic stop_;
};

} // namespace sndio
} // namespace roc

#endif // ROC_SNDIO_RECORDER_H_
