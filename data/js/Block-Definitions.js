var NUM_BLOCKS = 19;
function getBlockHtml(index, Code) {
    switch (Code[index]) {
        case 1:
            return "Turn Motor by \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\"> \
            degrees, Hub: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"7\">, \
            Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"5\">";
        case 2:
            return "Set Motor speed to \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\">\
            %, HUB: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"7\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"5\">";
        case 3:
            return "Run Motor for \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\">\
            milliseconds, HUB: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"7\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"5\">";
        case 4:
            return "Turn Motor to absolute posision \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\">\
            °, HUB: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"7\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"5\">";
        case 5:
            return "Set Motor encoder to \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\">\
            °, HUB: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"7\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"5\">";
        case 7:
            return "var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            equals to \
            ";
        case 8:
            return "var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            * var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 9:
            return "var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            / var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 10:
            return "var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            + var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 11:
            return "var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            - var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 12:
            return "If \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 13:
            return "var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            <div class=\"custom-select\"><select value=\"" + Code[index + 2] + "\" name=\"bool\" id=\"bool_" + (index + 2) + "\" onchange=\"selectArgChange(this)\">\
                <option value=\"0\">equals</option>\
                <option value=\"1\">greater than</option>\
                <option value=\"2\">less than</option>\
                <option value=\"3\">or</option>\
                <option value=\"4\">and</option>\
                <option value=\"5\">not</option>\
            </select></div> \
            var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 14:
            return "<div class=\"custom-select\" style=\"width: max-content\"><select value=\"" + Code[index + 1] + "\" name=\"math\" id=\"math_" + (index + 1) + "\" onchange=\"selectArgChange(this)\">\
                <option value=\"0\">round</option>\
                <option value=\"1\">abs</option>\
                <option value=\"2\">sqrt</option>\
                <option value=\"3\">sin</option>\
                <option value=\"4\">cos</option>\
                <option value=\"5\">tan</option>\
                <option value=\"6\">log</option>\
                <option value=\"7\">ln</option>\
                <option value=\"8\">e^</option>\
                <option value=\"9\">10^</option>\
                <option value=\"10\">asin</option>\
                <option value=\"11\">acos</option>\
                <option value=\"12\">atan</option>\
                <option value=\"13\">floor</option>\
                <option value=\"14\">ceiling</option>\
            </select></div>\
            (var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            )";
        case 15:
            return "var\
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"255\">\
            Mod var \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"255\">";
        case 16:
            return "Set Motor PWM to \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\">\
            , Channel: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"3\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 3) + "_Arg\" value=\"" + Code[index + 3] + "\" onchange=\"ArgChange(this)\" max=\"1\">";
        case 17:
            return "Increment, Channel: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"3\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"1\">";
        case 18:
            return "Decrement, Channel: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 1) + "_Arg\" value=\"" + Code[index + 1] + "\" onchange=\"ArgChange(this)\" max=\"3\">\
            , Port: \
            <input type=\"number\" class=\"argInput\" id=\"" + (index + 2) + "_Arg\" value=\"" + Code[index + 2] + "\" onchange=\"ArgChange(this)\" max=\"1\">";
        case 19:
            return "var \
            ";
        default:
            return "Unknown"
    }
}
const BLOCK_NAME = [
    "Turn Motor by 0° degrees, HUB: 0, Port: 0",
    "Set Motor speed to 0%, HUB: 0, Port: 0",
    "Run Motor for 0 milliseconds, HUB: 0, Port: 0",
    "Turn Motor to absolute posision 0°, HUB: 0, Port: 0",
    "Set Motor encoder to 0°, HUB: 0, Port: 0",
    "",
    "var 0 equals to 0",
    "var 0 * var 0",
    "var 0 / var 0",
    "var 0 + var 0",
    "var 0 - var 0",
    "If 0",
    "var 0 equals? var 0",
    "round(var 0)",
    "var0 Mod var 0",
    "Set Motor PWM to 0, Channel: 0, Port: 0",
    "Increment, Channel: 0, Port: 0",
    "Decrement, Channel: 0, Port: 0",
    "var 0",
]
const BLOCK_COLOR = [
    "", // not a valid index
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "",
    "#ffa500",
    "#00a560",
    "#00a560",
    "#00a560",
    "#00a560",
    "#ffff00",
    "#00ffff",
    "#00a560",
    "#00a560",
    "#0080ff",
    "#0080ff",
    "#0080ff",
    "#ffa500",
]
const BLOCK_TEXT_COLOR = [
    "", // not a valid index
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#000000",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
    "#ffffff",
]
const BLOCK_LEN = [
    0, // not a valid index
    3,
    3,
    3,
    3,
    3,
    0,
    3,
    3,
    3,
    3,
    3,
    1,
    4,
    3,
    3,
    3,
    2,
    2,
    1,
]