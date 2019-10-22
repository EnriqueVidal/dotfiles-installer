open Bos;

let git = Cmd.v("git");

let rubyBuild = (env: Env.t): option(Env.t) => {
  let path = Env.getVar("RBENV_PATH", env) ++ "/plugins/ruby-build";
  let clone =
    Cmd.(
      git % "clone" % "https://github.com/sstephenson/ruby-build.git" % path
    );

  Console.log("Installing ruby-build");
  Core.exec(env, clone);
};

let rbenv = (env: Env.t): option(Env.t) => {
  let path = Env.getVar("RBENV_PATH", env);
  let clone =
    Cmd.(git % "clone" % "https://github.com/sstephenson/rbenv.git" % path);

  Console.log("Installing rbenv");
  Core.exec(env, clone);
};

let install = (env: Env.t) => Core.flatMap(rbenv(env), rubyBuild);
