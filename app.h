class App {  
  App(const App&)= delete;
  App(App&& app) = delete;
  App& operator=(const App& app) = delete;

  public:
  App();
  void Run();
  ~App();
};
