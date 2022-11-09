#! /bin/bash

source ~/.profile
source ~/.config/zsh/env.zsh
this=_icons
s2d_reset="^d^"
color="^c#223344^^b#4E5173^"
signal=$(echo "^s$this^" | sed 's/_//')

update() {
    icons=()
    [ "$(ps -aux | grep 'aria2' | sed 1d)" ] && icons=(${icons[@]} "")
    [ "$(docker ps | grep 'v2raya')" ] && icons=(${icons[@]} "")

    sed -i '/^export '$this'=.*$/d' $DWM/statusbar/temp
    if [ "$icons" ]; then
        text=" ${icons[@]} "
        printf "export %s='%s%s%s%s'\n" $this "$color" "$signal" "$text" "$s2d_reset" >> $DWM/statusbar/temp
    fi
}

click() { :; }

case "$1" in
    click) click $2 ;;
    *) update ;;
esac
