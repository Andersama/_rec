// _rec.cpp : Defines the entry point for the application.
//

#include "elements.hpp"
#include "elements/element/text.hpp"

#include "q_io/audio_device.hpp"
#include "q_io/midi_device.hpp"
#include "portaudio.h"

#include "fmt/format.h"

#include "_rec.h"

using namespace cycfi::elements;
namespace q = cycfi::q;

int main(int argc, char *argv[]) {
    auto audio_list = q::audio_device::list();
    auto midi_list  = q::midi_device::list();

    size_t host_apis = Pa_GetHostApiCount();

    std::string log = "";
    log.reserve(audio_list.size() * 256);

    auto constexpr bkd_color = rgba(35, 35, 37, 255);
    auto background          = box(bkd_color);

    auto txt_bit = share(cycfi::elements::static_text_box(log, font{}, 9.0, rgba(255, 255, 255, 255)));
    auto scroll  = share(scroller(hold(txt_bit)));

    auto audio_btn = button("audio devices");
    audio_btn.on_click = [&log, &txt_bit](bool) mutable {
        auto audio_list = q::audio_device::list();
        for (auto const &device : audio_list) {
            fmt::format_to(std::back_inserter(log), "{}", "=========================\n");
            fmt::format_to(std::back_inserter(log),
                           "id: {}\n"
                           "name: {}\n"
                           "inputs: {}\n"
                           "outputs: {}\n",
                           device.id(), device.name(), device.input_channels(), device.output_channels());
        }
        txt_bit->set_text(log);
        std::cout << log << '\n';
        log.clear();
    };

    auto midi_btn     = button("midi devices");
    midi_btn.on_click = [&log, &txt_bit](bool) mutable {
        auto midi_list = q::midi_device::list();
        for (auto const &device : midi_list) {
            fmt::format_to(std::back_inserter(log), "{}", "=========================\n");
            fmt::format_to(std::back_inserter(log),
                           "id: {}\n"
                           "name: {}\n"
                           "inputs: {}\n"
                           "outputs: {}\n",
                           device.id(), device.name(), device.num_inputs(), device.num_outputs());
        }
        txt_bit->set_text(log);
        std::cout << log << '\n';
        log.clear();
    };

    auto host_btn = button("host");
    host_btn.on_click = [&log, &txt_bit](bool) mutable {
        size_t host_apis = Pa_GetHostApiCount();
        for (size_t i = 0; i < host_apis; i++) {
            auto api_info = Pa_GetHostApiInfo(i);
            fmt::format_to(std::back_inserter(log), "{}", "=========================\n");
            fmt::format_to(std::back_inserter(log),
                           "name: {}\n"
                           "struct version: {}\n"
                           "devices: {}\n",
                           api_info->name, api_info->structVersion, api_info->deviceCount);
        }
        txt_bit->set_text(log);
        std::cout << log << '\n';
        log.clear();
    };

    log.clear();

    auto btn_bar = htile(audio_btn, midi_btn, host_btn);
    auto wndw    = vtile(btn_bar, hold(scroll));

    app    _app(argc, argv, "_rec", "_rec");
    window _win(_app.name());

    _win.on_close = [&_app]() {
        _app.stop();
    };

    view _view(_win);
    _view.content(wndw, background);

    _app.run();

    return 0;
}
