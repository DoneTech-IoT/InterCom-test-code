# Logging Best Practices

## Designing Log Messages

To design effective log messages, consider the following questions:
- **What information would you need to resolve an issue?**
- **Which logs are useful to understand system behavior without remote access?**
- **What information is necessary for automated issue mitigation?**
- **What actions should others take when they read the log?**

Remember, logs should be written for other developers, not just yourself.

## Best Practices for Logging

### Search/Analysis
- **Unique Message**: Ensure log messages are unique within a microservice to facilitate easy identification and analysis.
- **Time**: Include the time from bootup in your log messages.
- **Stack Trace**: Always log the full stack trace for exceptions to identify root causes accurately.
- **Actionable Logging**: Write understandable logs and provide clear action plans for the reader.
- **Typed/Prefixed Logging**: Use typed or prefixed logging to differentiate between different functionalities within the same service.

### Event
- **Event Started**: Log the start of significant events to help diagnose issues in production.
- **Deployment Time**: Log deployment times to help identify issues that arise after a new deployment.
- **External Interface**: Log interactions with external interfaces, such as APIs and file I/O, to monitor and troubleshoot boundary issues.

### State
- **Aggregation**: Aggregate detailed logs to avoid verbosity while still capturing necessary information.
