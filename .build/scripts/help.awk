{
    if ($0 ~ /^[a-zA-Z\-_0-9.]+:/) {
        helpCommand = substr($0, 0, index($0, ":"));
        if (helpMessage) {
            printf "\033[94m%-22s\033[0m %s\n", helpCommand, helpMessage;
            helpMessage = "";
        }
    } else if ($0 ~ /^##/) {
        if (helpMessage) {
            helpMessage = helpMessage"\n                       "substr($0, 3);
        } else {
            helpMessage = substr($0, 3);
        }
    } else {
        if (helpMessage) {
          gsub(/^[ \t]+/, "", helpMessage);
          print "\n\033[36m"helpMessage"\033[0m\n";
        }
        helpMessage = "";
    }
}