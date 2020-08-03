/**
 * ---
 * --- THIS NEEDS TO/SHOULD BE CHANGED BY YOU!!!
 * ---
 *
 * This needs to match the following line in `garage_door_opener.ino`:
 * `const char POST_SECRET[] = "<some_madeup_secret_for_pseudo_security>";`
 */

const POST_SECRET = "<some_madeup_secret_for_pseudo_security>";

/**
 * "INITIALIZE" - Loading method
 */

// Set up misc handlers
$("#alert-error-close-btn").on("click", (event) => {
  $("#alert-error").hide();
});

// Make sure our error alert is hidden
$("#alert-error").hide();

// Fetch status of our garage door
(async () => {
  const resp = await fetch("/status");
  const isGarageOpen = await resp.text();

  let garageStatus = "CLOSED",
    isOpenGarageDoorBtnDisabled = false,
    isCloseGarageDoorBtnDisabled = true;

  if (isGarageOpen === "true") {
    garageStatus = "OPEN";
    isCloseGarageDoorBtnDisabled = false;
    isOpenGarageDoorBtnDisabled = true;
  }

  $("#garage-door-status").html(garageStatus);
  $("#open-garage-door-btn").prop("disabled", isOpenGarageDoorBtnDisabled);
  $("#close-garage-door-btn").prop("disabled", isCloseGarageDoorBtnDisabled);
})();

/**
 * OPEN GARAGE DOOR BUTTON HANDLER
 */

$("#open-garage-door-btn").on("click", (event) => {
  $("#open-garage-door-btn").html("OPENING GARAGE DOOR...");

  (async () => {
    try {
      const resp = await fetch("/open_garage", {
        method: "POST",
        body: POST_SECRET,
      });
      const success = await resp.text();

      if (success === "true") {
        $("#garage-door-status").html("OPEN");
        $("#open-garage-door-btn").prop("disabled", true);
        $("#close-garage-door-btn").prop("disabled", false);
      }

      $("#open-garage-door-btn").html("OPEN GARAGE DOOR");
    } catch (e) {
      $("#alert-error-message").html(
        "Unable to open garage. Please make sure you are connected to WiFi then refresh this page and try again."
      );
      $("#alert-error").show();
      $("#open-garage-door-btn").html("OPEN GARAGE DOOR");
    }
  })();
});

/**
 * CLOSE GARAGE DOOR BUTTON HANDLER
 */

$("#close-garage-door-btn").on("click", (event) => {
  $("#close-garage-door-btn").html("CLOSING GARAGE DOOR...");

  (async () => {
    try {
      const resp = await fetch("/close_garage", {
        method: "POST",
        body: POST_SECRET,
      });
      const success = await resp.text();

      if (success === "true") {
        $("#garage-door-status").html("CLOSED");
        $("#close-garage-door-btn").prop("disabled", true);
        $("#open-garage-door-btn").prop("disabled", false);
      }

      $("#close-garage-door-btn").html("CLOSE GARAGE DOOR");
    } catch (e) {
      $("#alert-error-message").html(
        "Unable to close garage. Please make sure you are connected to WiFi then refresh this page and try again."
      );
      $("#alert-error").show();
      $("#close-garage-door-btn").html("CLOSE GARAGE DOOR");
    }
  })();
});
