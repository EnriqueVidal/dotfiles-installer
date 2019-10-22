open Bos;

let apt = Cmd.v("apt-get");
let curl = Cmd.v("curl");
let sudo = Cmd.v("sudo");
let which = Cmd.v("which");

/* TODO: Sanitize this list to only include bare essentials */
let packages = [
  "build-essential",
  "curl",
  "git",
  "ruby",
  "texinfo",
  "libbz2-dev",
  "libcurl4-openssl-dev",
  "libexpat-dev",
  "libncurses-dev",
  "zlib1g-dev",
  "libreadline-dev",
  "libssl-dev",
];

module Brew = {
  let home = "~/.linuxbrew";
  let path = home ++ "/Homebrew";
  let repo = "https://github.com/Homebrew/brew";

  let clone = Cmd.(v("git") % "clone" % repo % path);
  let mkdir = Cmd.(v("mkdir") % (home ++ "/bin"));
  let softl = Cmd.(v("ln") % "-s" % (path ++ "/brew") % (home ++ "/bin"));

  let post = (env: Env.t): option(Env.t) => {
    let rcfile = Fpath.v(Env.getVar("HOME", env) ++ "/.bash_profile.before");
    let content = "eval $(" ++ home ++ "/bin/brew shellenv)";

    switch (Core.fileAppend(rcfile, content)) {
    | Result.Ok () => Some(env)
    | _ => None
    };
  };

  let install = (env: Env.t): option(Env.t) => {
    let result = [clone, mkdir, softl] |> Core.inBand(env);

    switch (result) {
    | Result.Ok () => post(env)
    | _ =>
      Console.log("Linuxbrew installation failed");
      None;
    };
  };
};

let update = (env: Env.t): option(Env.t) =>
  Cmd.(sudo %% apt % "update") |> Core.exec(env);

let pkg = (env: Env.t): option(Env.t) => {
  let install = Cmd.(sudo %% apt % "-qq" % "install" % "-y");

  Cmd.of_list(packages) |> Cmd.add_args(install) |> Core.exec(env);
};

let install = (env: Env.t): option(Env.t) => {
  let debian = Cmd.(which %% apt) |> OS.Cmd.run_out |> OS.Cmd.to_string;

  Console.log("Installing Linux dependencies");

  switch (debian) {
  | Result.Ok(_) => Core.flatMap(update(env), pkg)
  | _ =>
    Console.log("At the moment only debian based distros supported");
    None;
  };
};
