import 'dart:async';

import 'package:flutter/material.dart';

typedef _AsyncBoolCallback = Future<bool> Function();

// ignore: must_be_immutable
class TimeoutButton extends StatelessWidget {
  final _enabled = ValueNotifier(false);
  Timer _timer = Timer(Duration(seconds: 0), () {});
  final _AsyncBoolCallback callback;

  TimeoutButton({@required this.callback});

  Widget build(BuildContext context) {
    return ValueListenableBuilder(
        valueListenable: _enabled,
        builder: (BuildContext context, bool enabled, Widget child) => Ink(
              decoration: ShapeDecoration(
                color: enabled ? Colors.blue : Colors.grey,
                shape: CircleBorder(),
              ),
              child: IconButton(
                icon: Icon(Icons.sensor_door),
                padding: new EdgeInsets.all(16),
                iconSize: 96,
                color: Colors.white,
                onPressed: () {
                  onPressed();
                },
              ),
            ));
  }

  onPressed() async {
    final success = await callback();
    _enabled.value = success;
    if (success) {
      _timer.cancel();
      _timer = Timer(Duration(seconds: 5), () {
        _enabled.value = false;
      });
    }
  }
}
