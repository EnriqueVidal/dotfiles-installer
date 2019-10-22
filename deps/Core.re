open Bos;

type installer = Env.t => Cmd.t;

let exec = (env: Env.t, cmd: Cmd.t): option(Env.t) => {
  let result = OS.Cmd.run(~env, cmd);

  switch (result) {
  | Result.Ok () => Some(env)
  | _ => None
  };
};

let flatMap = (x, y) =>
  switch (x) {
  | Some(x') => y(x')
  | _ => None
  };

let fileAppend = (path: Fpath.t, content: string): OS.result('a, 'b) =>
  switch (OS.File.read(path)) {
  | Result.Ok(oldContent) =>
    OS.File.write(path, oldContent ++ "\n" ++ content)
  | _ => OS.File.write(path, content)
  };

let toCommands =
    (env: Env.t, installlers: list(installer)): option(list(Cmd.t)) =>
  None;

let rec inBand = (env: Env.t, commands: list(Cmd.t)): OS.result('a, 'b) =>
  switch (commands) {
  | [] => Result.Ok()
  | [command, ...rest] =>
    switch (OS.Cmd.run(~env, command)) {
    | Result.Ok () => inBand(env, rest)
    | error => error
    }
  };

let install = () => {};
